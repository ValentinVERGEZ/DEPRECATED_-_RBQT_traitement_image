RBQT_traitement_image
=====================

Stack ROS de traitement d'image pour l'équipe RBQT dans le cadre de la Robocup

Le code est brouillon à ce stade.

Seul le package lecture_feu_tricolore a vu son développement commencer.
* Le bin test_opencv_bridge n'a pas d'interêt pour la Robocup, c'est un test des possibilités ROS/OpenCV
* Le bin publisher permet de simuler un topic \image_raw avec possibilité de clignotement
* Le bin lecture_feu lit le topic \image_raw et publie le résultat sur \rbqt_lecture_feu_tricolore\result.
