#include <rbqt_lecture_feu_tricolore/traitement_feu.h>


int main(int argc, char** argv)
{
    ros::init(argc, argv, "lecture_feu");
    TraitementFeu tf;
    ros::Rate loop_rate(5);
    while (tf.ok()) 
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}