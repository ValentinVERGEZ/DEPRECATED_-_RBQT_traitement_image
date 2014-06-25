/*==========  Inludes  ==========*/
#include <rbqt_lecture_feu_tricolore/traitement_feu.h>

/*==========  Global Variables  ==========*/
// DEFAULT VALUES
// // Red
//     int H_MIN_red = HSV_MIN;
//     int H_MAX_red = HSV_MAX;
//     int S_MIN_red = HSV_MIN;
//     int S_MAX_red = HSV_MAX;
//     int V_MIN_red = HSV_MIN;
//     int V_MAX_red = HSV_MAX;

// // Green
//     int H_MIN_green = HSV_MIN;
//     int H_MAX_green = HSV_MAX;
//     int S_MIN_green = HSV_MIN;
//     int S_MAX_green = HSV_MAX;
//     int V_MIN_green = HSV_MIN;
//     int V_MAX_green = HSV_MAX;

// // Orange
//     int H_MIN_orange = HSV_MIN;
//     int H_MAX_orange = HSV_MAX;
//     int S_MIN_orange = HSV_MIN;
//     int S_MAX_orange = HSV_MAX;
//     int V_MIN_orange = HSV_MIN;
//     int V_MAX_orange = HSV_MAX;

// STANDARD VALUES
// Red
    int H_MIN_red = 173;
    int H_MAX_red = 179;
    int S_MIN_red = 114;
    int S_MAX_red = HSV_MAX;
    int V_MIN_red = HSV_MIN;
    int V_MAX_red = HSV_MAX;
    int V_SEUIL_red = 150;

// Green
    int H_MIN_green = 78;
    int H_MAX_green = 93;
    int S_MIN_green = 115;
    int S_MAX_green = HSV_MAX;
    int V_MIN_green = HSV_MIN;
    int V_MAX_green = HSV_MAX;
    int V_SEUIL_green = 25;

// Orange
    int H_MIN_orange = 0;
    int H_MAX_orange = 12;
    int S_MIN_orange = 150;
    int S_MAX_orange = HSV_MAX;
    int V_MIN_orange = HSV_MIN;
    int V_MAX_orange = HSV_MAX;
    int V_SEUIL_orange = 173;

/*========================================
=            Class Definition            =
========================================*/


TraitementFeu::TraitementFeu()
  : it_(nh_)
{
    image_pub_ = it_.advertise("test_out", 1);
    image_sub_ = it_.subscribe("image_raw", 1, &TraitementFeu::imageCb, this);

    createTrackbars();
    // cv::namedWindow(WINDOW);
    cv::namedWindow("Origin");
    // cv::namedWindow("BGR");
    // cv::namedWindow("Result");
    cv::namedWindow("Red");
    cv::namedWindow("Orange");
    cv::namedWindow("Green");

    cv::resizeWindow("Red",320,240*3-86);
    cv::resizeWindow("Orange",320,240*3-86);
    cv::resizeWindow("Green",320,240*3-86);

    cv::moveWindow("Red",0,0);
    cv::moveWindow("Orange",320,0);
    cv::moveWindow("Green",320*2,0);
}

TraitementFeu::~TraitementFeu()
{

}

/**
 * @brief CallBack fonction
 * @details [long description]
 * 
 * @param msg [description]
 */
void TraitementFeu::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
// Environment
    cv::Mat origin, hsv, bgr, red, orange, green, result;

// Algo
    // Get image
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv_ptr->image.copyTo(origin);
    cv_ptr->image.copyTo(bgr);
    cv_ptr->image.copyTo(hsv);

    // Convert
    cv::cvtColor(origin,hsv,CV_RGB2HSV);
    cv::cvtColor(origin,bgr,CV_RGB2BGR);

    origin.copyTo(red);
    origin.copyTo(green);
    origin.copyTo(orange);
    origin.copyTo(result);

    // Threshold
    cv::inRange(hsv,cv::Scalar(H_MIN_red,S_MIN_red,V_MIN_red),cv::Scalar(H_MAX_red,S_MAX_red,V_MAX_red),red); 
    morphOps(red);
    cv::inRange(hsv,cv::Scalar(H_MIN_green,S_MIN_green,V_MIN_green),cv::Scalar(H_MAX_green,S_MAX_green,V_MAX_green),green);
    morphOps(green);
    cv::inRange(hsv,cv::Scalar(H_MIN_orange,S_MIN_orange,V_MIN_orange),cv::Scalar(H_MAX_orange,S_MAX_orange,V_MAX_orange),orange);
    morphOps(orange);

    // Threshold Result
    cv::Mat res_red, res_orange, res_green;
    cv::Mat in_red[] = {red, red, red};
    cv::Mat in_orange[] = {orange, orange, orange};
    cv::Mat in_green[] = {green, green, green};
    cv::merge(in_red, 3, res_red);
    cv::merge(in_orange, 3, res_orange);
    cv::merge(in_green, 3, res_green);
    cv::bitwise_and(bgr, res_red, res_red);
    cv::bitwise_and(bgr, res_orange, res_orange);
    cv::bitwise_and(bgr, res_green, res_green);

    // Luminance
    int mean_value_r, mean_value_o, mean_value_g;

    mean_value_r = cv::sum(res_red)[2]/cv::countNonZero(red);
    cv::putText(res_red,"Mean : "+intToString(mean_value_r)
        +((mean_value_r>V_SEUIL_red)?"  !! ON !!":"  ...  ...")
        ,cv::Point(0,20),1,2,cv::Scalar(255,255,255),2);
    mean_value_o = cv::sum(res_orange)[2]/cv::countNonZero(orange);
    cv::putText(res_orange,"Mean : "+intToString(mean_value_o)
        +((mean_value_o>V_SEUIL_orange)?"  !! ON !!":"  ...  ...")
        ,cv::Point(0,20),1,2,cv::Scalar(255,255,255),2);
    mean_value_g = cv::sum(res_green)[2]/cv::countNonZero(green);
    cv::putText(res_green,"Mean : "+intToString(mean_value_g)
        +((mean_value_g>V_SEUIL_green)?"  !! ON !!":"  ...  ...")
        ,cv::Point(0,20),1,2,cv::Scalar(255,255,255),2);

    // Show
    // cv::imshow(WINDOW,cv_ptr->image);
    cv::imshow("Origin", bgr);
    // cv::imshow("BGR", bgr);
    cv::imshow("Red", res_red);
    cv::imshow("Orange", res_orange);
    cv::imshow("Green", res_green);
    // cv::imshow("Result", result);
    cv::waitKey(3);
    
    cv_ptr->image = result;
    image_pub_.publish(cv_ptr->toImageMsg());

}

void TraitementFeu::segmentation()
{

}

void TraitementFeu::traitement()
{

}

bool TraitementFeu::ok()
{
    return nh_.ok();
}

/*-----  End of Class Definition  ------*/


void on_trackbar( int, void* ){
    //This function gets called whenever a
    // trackbar position is changed
}

void createTrackbars()
{
    //create window for trackbars
    cvNamedWindow(trackbarWindowName_r,0);
    cvNamedWindow(trackbarWindowName_o,0);
    cvNamedWindow(trackbarWindowName_g,0);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH), 
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)                                ---->    ---->     ---->      
    cv::createTrackbar("H_MIN_r", trackbarWindowName_r, &H_MIN_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("H_MAX_r", trackbarWindowName_r, &H_MAX_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MIN_r", trackbarWindowName_r, &S_MIN_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MAX_r", trackbarWindowName_r, &S_MAX_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MIN_r", trackbarWindowName_r, &V_MIN_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MAX_r", trackbarWindowName_r, &V_MAX_red, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_SEUIL_r", trackbarWindowName_r, &V_SEUIL_red, HSV_MAX, on_trackbar);

    cv::createTrackbar("H_MIN_g", trackbarWindowName_g, &H_MIN_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("H_MAX_g", trackbarWindowName_g, &H_MAX_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MIN_g", trackbarWindowName_g, &S_MIN_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MAX_g", trackbarWindowName_g, &S_MAX_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MIN_g", trackbarWindowName_g, &V_MIN_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MAX_g", trackbarWindowName_g, &V_MAX_green, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_SEUIL_g", trackbarWindowName_g, &V_SEUIL_green, HSV_MAX, on_trackbar);

    cv::createTrackbar("H_MIN_o", trackbarWindowName_o, &H_MIN_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("H_MAX_o", trackbarWindowName_o, &H_MAX_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MIN_o", trackbarWindowName_o, &S_MIN_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("S_MAX_o", trackbarWindowName_o, &S_MAX_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MIN_o", trackbarWindowName_o, &V_MIN_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_MAX_o", trackbarWindowName_o, &V_MAX_orange, HSV_MAX, on_trackbar);
    cv::createTrackbar("V_SEUIL_o", trackbarWindowName_o, &V_SEUIL_orange, HSV_MAX, on_trackbar);
}


/* effectue le traitement de l'image binaire pour obtenir un objet plus fidèle à l'objet réel */
void morphOps(cv::Mat &mask){ //en paramètre le flux à traiter

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px ellipse
    cv::Mat erodeElement = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    cv::Mat dilateElement = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)); //ancien 3.3

    // Ouverture anti-parasites
    for(int k=0;k<3;k++){
        erode(mask,mask,erodeElement);
        dilate(mask,mask,dilateElement);
    }


    cv::Mat erodeElement2 = getStructuringElement(cv::MORPH_RECT,cv::Size(25,25));
    //dilate with larger element so make sure object is nicely visible
    cv::Mat dilateElement2 = getStructuringElement(cv::MORPH_RECT,cv::Size(25,25)); //ancien 3.3

    // Fermeture pour luminositée
    for(int k=0;k<100;k++){
        dilate(mask,mask,dilateElement2);
        erode(mask,mask,erodeElement2);
    }
}

std::string intToString(int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}