#ifndef _RBQT_LECTURE_FEU_TRICOLORE__TRAITEMENT_FEU__HEADER_
#define _RBQT_LECTURE_FEU_TRICOLORE__TRAITEMENT_FEU__HEADER_

/*==========  Includes  ==========*/
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/time.h>
#include <sstream>

#include "std_msgs/String.h"

/*==========  Namespaces  ==========*/
namespace enc = sensor_msgs::image_encodings;

/*==========  Constantes  ==========*/
static const char WINDOW[] = "Image window";
// static const char trackbarWindowName_r[] = "Trackbars for Red threshold";
// static const char trackbarWindowName_o[] = "Trackbars for Orange threshold";
// static const char trackbarWindowName_g[] = "Trackbars for Green threshold";
static const char trackbarWindowName_r[] = "Red";
static const char trackbarWindowName_o[] = "Orange";
static const char trackbarWindowName_g[] = "Green";

/*==========  Defines  ==========*/
#define HSV_MIN 0
#define HSV_MAX 255

/*==========  Types  ==========*/
typedef struct 
{
    int state;
    long long int timeSinceLastChange;
    long long int timeSincePrecChange;
    int computedState;
}Feu;

/*=========================================
=            Class Declaration            =
=========================================*/

class TraitementFeu
{
private:
    // ROS
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    ros::Publisher publisher_;
  
    // OpenCV    
    cv::Mat _origin, _hsv, _bgr, _red, _orange, _green, _result;

    // Traitement
    Feu _lightRed, _lightOrange, _lightGreen;

public:
    TraitementFeu();
    ~TraitementFeu();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);
    void traitement();
    bool ok();
};

/*-----  End of Class Declaration  ------*/

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param t [description]
 * @param d [description]
 */
void on_trackbar( int, void* );

/**
 * @brief [brief description]
 * @details [long description]
 */
void createTrackbars();

/* effectue le traitement de l'image binaire pour obtenir un objet plus fidèle à l'objet réel */
/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param mask [description]
 */
void morphOps(cv::Mat &mask);

/**
 * @brief Convert int to string
 * 
 * @param number Number to be converted
 * @return Number converted into string format
 */
std::string intToString(int number);

#endif // _RBQT_LECTURE_FEU_TRICOLORE__TRAITEMENT_FEU__HEADER_