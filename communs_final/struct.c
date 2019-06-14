Path *generate_path(unsigned char *color, int max_width, int max_height, int x_sight, int y_sight){
    if(!strcmp(color, "yellow") || !strcmp(color, "blue")){
        return generate_main_path(max_width, max_height);
    }
    if(!strcmp(color, "green") || !strcmp(color, "purple")){
        return generate_secondary_path(max_width, max_height, x_sight, y_sight);
    }
    return NULL;
}

Path* generate_main_path(int max_width, int max_height) {
    int xmin = max_width % (4 * BLUE_SIGHT), ymin = 0,
        xmax = max_width, ymax = max_height,
        inter_nb = max_width / (4 * BLUE_SIGHT),
        i;

    Path *head = create_path();

    Point point1 = create_point(xmin + BLUE_SIGHT, ymin + BLUE_SIGHT),
          point2 = create_point(xmax - BLUE_SIGHT, ymin + BLUE_SIGHT),
          point3 = create_point(xmax - BLUE_SIGHT, ymax - BLUE_SIGHT),
          point4 = create_point(xmin + BLUE_SIGHT, ymax - BLUE_SIGHT);

    add_point(&head, point1);
    add_point(&head, point2);
    add_point(&head, point3);

    for (i = 0; i < inter_nb - 1; i++) {
        Point p1 = create_point(xmax - BLUE_SIGHT * (4 * i + 3), ymax - BLUE_SIGHT),
              p2 = create_point(xmax - BLUE_SIGHT * (4 * i + 3), ymin + BLUE_SIGHT * 3),
              p3 = create_point(xmax - BLUE_SIGHT * (4 * i + 5), ymin + BLUE_SIGHT * 3),
              p4 = create_point(xmax - BLUE_SIGHT * (4 * i + 5), ymax - BLUE_SIGHT);
        
        add_point(&head, p1);
        add_point(&head, p2);
        add_point(&head, p3);
        add_point(&head, p4);
    }

    add_point(&head, point4);

    return head;
}


Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight){
    int zone_min_x, zone_max_x, zone_min_y, zone_max_y, i, i_max;
    Path *head = create_path();
    Point point1, point2, point3, point4, p1, p2, p3, p4, p_comp1, p_comp2, p_comp3, p_comp4;

    zone_min_x = 0;
    zone_min_y = 0;
    zone_max_x = max_width%(4*BLUE_SIGHT);
    zone_max_y = max_height;

    if(zone_max_x == zone_min_x){
        return generate_main_path(max_width,max_height);
    }

    if( (zone_max_x - zone_min_x) <= (2*x_sight) ){
        point1 = create_point(zone_max_x/2, zone_min_y + y_sight);
        point2 = create_point(zone_max_x/2, zone_max_y - y_sight);
        add_point(&head, point1);
        add_point(&head, point2);
        return head;
    }

    if( (zone_max_y - zone_min_y) <= (2*y_sight) ){
        point1 = create_point(zone_min_x + x_sight, zone_max_y/2);
        point2 = create_point(zone_max_x - x_sight, zone_max_y/2);
        add_point(&head, point1);
        add_point(&head, point2);
        return head;
    }

    point1 = create_point(zone_min_x + x_sight, zone_min_y + y_sight);
    point2 = create_point(zone_max_x - x_sight, zone_min_y + y_sight);
    point3 = create_point(zone_max_x - x_sight, zone_max_y - y_sight);
    point4 = create_point(zone_min_x + x_sight, zone_max_y - y_sight);
    add_point(&head, point1);
    add_point(&head, point2);
    add_point(&head, point3);

    if( (zone_max_y - zone_min_y) <= (4*y_sight) ){
        add_point(&head, point4);
        return head;
    }

    i_max = ((zone_max_x - zone_min_x)/(4*x_sight))-1;
    for( i = 0 ; i < i_max ; i++){
        p1 = create_point(zone_max_x - x_sight * (4 * i + 3), zone_max_y - y_sight),
        p2 = create_point(zone_max_x - x_sight * (4 * i + 3), zone_min_y + y_sight * 3),
        p3 = create_point(zone_max_x - x_sight * (4 * i + 5), zone_min_y + y_sight * 3),
        p4 = create_point(zone_max_x - x_sight * (4 * i + 5), zone_max_y - y_sight);
        
        add_point(&head, p1);
        add_point(&head, p2);
        add_point(&head, p3);
        add_point(&head, p4);
    }

    if((zone_max_x-zone_min_x)%(4*x_sight) != (zone_max_x-zone_min_x)){
        if( (zone_max_x-zone_min_x)%(4*x_sight) > (2*x_sight)){
            p_comp1 = create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight),
            p_comp2 = create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_min_y + y_sight * 3),
            p_comp3 = create_point(zone_max_x - x_sight * (4 * i_max + 5), zone_min_y + y_sight * 3),
            p_comp4 = create_point(zone_max_x - x_sight * (4 * i_max + 5), zone_max_y - y_sight);
            add_point(&head, p_comp1);
            add_point(&head, p_comp2);
            add_point(&head, p_comp3);
            add_point(&head, p_comp4);
        }
        else if( (zone_max_x-zone_min_x)%(4*x_sight) > 0 ){
            p_comp1 = create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight),
            p_comp2 = create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_min_y + y_sight * 3),
            p_comp3 = create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight),
            add_point(&head, p_comp1);
            add_point(&head, p_comp2);
            add_point(&head, p_comp3);
        }
    }
    add_point(&head, point4);

    return head;
}

void sheep_count(Dog* dog, NodeList** head){
  NodeList* pointer = *head;
  Node n;
  while(pointer != NULL){
    n = pointer->node;
    if(strncmp("bot",n.nickname,strlen("bot"))==0){
      if(get_nodelist_portion(&dog->sheeps, n.id) == NULL){
        add_node(&dog->sheeps, n);
      }else{
        update_node(&dog->sheeps,n);
      }
    }
    pointer = pointer->next;
  }
}
