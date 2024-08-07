from ultralytics import YOLO
from ultralytics.engine.results import Results

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image as rosimg
import cv_bridge

from bboxes_ex_msgs.msg import BoundingBox
from bboxes_ex_msgs.msg import BoundingBoxes

class yolo_node(Node):
    def __init__(self):
        super().__init__('yolo_node')
        self.declare_parameter('model_path', 'yolov8n.pt')
        self.declare_parameter('device', 'cuda:0')
        self.declare_parameter('conf', 0.7)

        self.model_path = self.get_parameter('model_path').get_parameter_value().string_value
        self.device = self.get_parameter('device').get_parameter_value().string_value
        self.conf = self.get_parameter('conf').get_parameter_value().double_value


        self.model = YOLO(self.model_path)
        
        self.subscription = self.create_subscription(
            rosimg,
            'input/raw_image',
            self.listener_callback,0)
        self.subscription

        self.publisher_result = self.create_publisher(BoundingBoxes, 'output/result', 0)

        self.bridge = cv_bridge.CvBridge()

        self.get_logger().info('start ultra_obejct_detection package rt_detr node')
 
    def parse_boxes(self, results: Results) -> BoundingBoxes:
        bbox_array_msg = BoundingBoxes()

        for box_data in results.boxes:
            msg = BoundingBox()

            # get boxes values
            box = box_data.xywh[0]
            xmin = max(0, min(int(box[0] - box[2] / 2), 65535))
            ymin = max(0, min(int(box[1] - box[3] / 2), 65535))
            xmax = max(0, min(int(box[0] + box[2] / 2), 65535))
            ymax = max(0, min(int(box[1] + box[3] / 2), 65535))
            
            msg.xmin = xmin
            msg.ymin = ymin
            msg.xmax = xmax
            msg.ymax = ymax

            msg.class_id_int = int(box_data.cls)
            msg.class_id = self.model.names[int(box_data.cls)]
            msg.probability = float(box_data.conf)
            
            msg.id = int(box_data.id) if box_data.id is not None else 0
            bbox_array_msg.bounding_boxes.append(msg)

        return bbox_array_msg


    def listener_callback(self, msg):
        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        results = self.model.track(
            source=cv_image,
            device=self.device,
            verbose=False,
            persist=True,
            tracker="botsort.yaml",
            conf=self.conf,
            half=True)
        if len(results) > 0 and results[0].boxes:
            results: Results = results[0].cpu()
            bbox_msg = self.parse_boxes(results)
            bbox_msg.image_header = msg.header
            self.publisher_result.publish(bbox_msg)
            
        else:
            self.publisher_result.publish(BoundingBoxes())            


def main(args=None):
    rclpy.init(args=args)
    minimal_subscriber = yolo_node()
    rclpy.spin(minimal_subscriber)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()