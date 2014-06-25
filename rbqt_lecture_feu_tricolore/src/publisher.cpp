#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

enum signal {s000, s001, s010, s011, s100, s101, s110, s111};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("image_raw", 1);


    cv_bridge::CvImage cv_im[8];
    for(int i = 0; i < 8; i++)
    {
        switch(i)
        {
            case 0:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/000.jpg", 1) ;
            break;

            case 1:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/001.jpg", 1) ;
            break;

            case 2:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/010.jpg", 1) ;
            break;

            case 3:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/011.jpg", 1) ;
            break;

            case 4:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/100.jpg", 1) ;
            break;

            case 5:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/101.jpg", 1) ;
            break;

            case 6:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/110.jpg", 1) ;
            break;

            case 7:
                cv_im[i].image = cv::imread("/home/leak/Projets/ros-pkg/rbqt_traitement_image/rbqt_lecture_feu_tricolore/img/screen_robotino/111.jpg", 1) ;
            break;
        }
        cv_im[i].encoding = sensor_msgs::image_encodings::BGR8;        
    }

    sensor_msgs::ImagePtr msg = cv_im[0].toImageMsg();
    ros::Rate loop_rate(5);
    while (nh.ok()) 
    {
        int choice = 0;
        std::cin >> choice;
        choice = ((choice<0)?0:((choice>7)?7:choice));

        sensor_msgs::ImagePtr msg = cv_im[choice].toImageMsg();
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
}