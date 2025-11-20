


    // This section just handles a manual frame advance mode. Can be removed without issue.
    /* I'm aware that this could be done much mdore cleanly, but I like the goto bodge job for aesthetic reasons.

    if (joypad() == J_RIGHT && frame_advance_mode == false){
      frame_advance_mode = true;
    }


    loop:
    keys = joypad();
    if (keys == J_LEFT){
        frame_advance_mode = false;
        goto advance;
    }
    
    if(frame_advance_mode == true && keys != J_LEFT){
      if(keys != J_RIGHT)
        goto loop;
    }

    
    advance:
    */