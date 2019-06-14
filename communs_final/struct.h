/*
* Generate a path using a dog's color
* color : color of the dog
* max_width : width of map
* max_height : height of map
*/
Path *generate_path(unsigned char *color, int max_width, int max_height, int x_sight, int y_sight);

/*
* Generate Blue & Yellow dogs path
* head : head of the path
* max_width : width of map
* max_height : height of map
*/
Path *generate_main_path(int max_width, int max_height);

/*
* Generate Green/Purple dogs path
* head : head of the path
* max_width : width of map
* max_height : height of map
*/
Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight);

void sheep_count(Dog* dog, NodeList** head);