-- --------------------------- LIBRARY

--[[
--  -----------------------  BLINK LIGHT
-- func - function
-- start - start time offset
-- repeats
-- duration
-- interval
 ]]
function blink(a)

    local interval = (a.interval or 0) + (a.duration or 0)

    clock_schedule({func=a.func,
        startOffset=a.start,
        infinite=false,
        repeats=a.repeats,
        interval=interval});

    clock_schedule({func=a.func,
        startOffset=a.start+a.duration,
        infinite=false,
        repeats=a.repeats,
        interval=interval});
end


--[[
--  -----------------------  SHAKE CAMERA
 ]]
do
    neg = true;
    local repeatsNum;
    expid = sound_create("explosion");
    function shakeCam()
        repeatsNum=20;
        clock_schedule({func="shakeCameraImpl",
            startOffset=0,
            repeats=repeatsNum,
            infinite=false,
            interval=50});
        cx,cy,cz = camera_getPosition();
        sound_play(expid);
    end

    function shakeCameraImpl()
        repeatsNum = repeatsNum - 1;
        neg = not neg;
        local offset;
        if neg then
            offset = - (repeatsNum+2)*0.1;
        else
            offset = repeatsNum*0.1;
        end
        camera_velocity(offset,offset,0);
        if repeatsNum<1 then
            camera_velocity(0,0,0);
        end
    end

end

-- ---------------------------------
-- -------------------- LEVEL
-- ---------------------------------

PROTAGONIST_UID = 0;


-- -----------------

function spawnLightToogle()
    blink({func="toggleLight",start=0,repeats=math.random(2,4),duration=50,interval=50});
end
do
    local enable = true;
    function toggleLight()
        enable = not enable
        object_enable("bl_light",enable)
    end
end


local animListenerId = 0;

do
    draw = true;
    function steam1Toggling()
        draw = not draw
        object_enable("steam1",draw)
        object_enable("e.s1",draw)

        object_enable("steam2", not draw)
        object_enable("e.s2", not draw)
    end
end

function lockCamera2Char()
    camera_lock(PROTAGONIST_UID,2,1.45)
end

function onStart()
    -- init globals --
    PROTAGONIST_UID = ai_getProtagonistUid();

    char_removeAnimation("idle");
    char_addAnimation("laying",true);

    input_block(true);
    input_addKeyPressed("wakeUp");

    clock_schedule({func="steam1Toggling",
        startOffset=1000,
        infinite=true,
        interval=1000});

    object_makeInteractive("button1", "openDoor", 1, 1);
    spawn1Uid = ai_findByName("spawnPnt1")

    lockCamera2Char();
end

function wakeUp(code, isDown)
    if isDown then
        input_removeKeyPressed("wakeUp")
        char_removeAnimation("laying");
        char_addAnimation("stand_up",false)
        animListenerId = char_addAnimationListener("animationListener")
    end
end

function animationListener(event)
    if event == "end" then
        char_addAnimation("idle",true)
        input_block(false)
        char_removeAnimationListener(animListenerId)
    end
end

-- ------------------- GROUND SHAKE AND BAR FALLS

function shake1()
    shakeCam();
    object_delete("barJoint");
    clock_schedule({func="shake2",
        startOffset=1000,
        infinite=false,
        repeats=1});
end

function shake2()
    object_delete("barJoint2");
    object_makeInteractive("papa", "grabBar", 1, 1);
end

-- --------------------- GRAB BAR

function grabBar()
    object_delete("papa")
    char_slotImg("slot1","sword")
    ai_setWeapon(PROTAGONIST_UID, 0, "bar")
end

function begDmg()
    char_hit(10);
end

function endDmg()
end

-- ------------------- BREAK FLOOR EVENT

function breakf()
    shakeCam();
    object_break("fp");
    light_dayTime(0.3);
    clock_schedule({func="spawnLightToogle", startOffset=500, infinite=true, interval=math.random(500,2000)})
end

-- --------------------- OPEN DOOR

do
    local up = true;
    local enable=true;
    function openDoor()
        if not enable then
            return
        end

        if up then
            object_setVelocity("door1",0,2);
        else
            object_setVelocity("door1",0,-2);
        end
        enable = false;
        up=not up;
        clock_schedule({func="stopDoorOpening", startOffset=500, repeats=1})

        light_dayTime(0.3);
    end

    function stopDoorOpening()
        enable = true;
        object_setVelocity("door1",0,0);
    end
end

-- ---------------- EVENT: spawn first enemy
do
    FIRST_ENEMY = 0;
    local sx,sy = 0,0

    function spFistE()
       -- grabBar();
        camera_lock(ai_findByName("clPnt1"),0,0)
        input_block(true);
        ai_stop(PROTAGONIST_UID)
        clock_schedule({func="spawnFirstEnemy", startOffset=750})
        clock_schedule({func="event_sfe_lockBack", startOffset=1750})
    end

    function spawnFirstEnemy()
        sx,sy = ai_getPosition(ai_findByName("spawnPnt1"))
        FIRST_ENEMY = ai_spawn("zombie",sx,sy);
        clock_postPhysStep({command='ai_onDestroyed(FIRST_ENEMY, "event_sfe_spawnMore")'})
    end

    function event_sfe_lockBack()
        lockCamera2Char();
        input_block(false);
    end

    function event_sfe_spawnMore()
        ai_spawn("zombie",sx+1.0,sy);
        ai_spawn("zombie",sx-2.5,sy);
    end
end
-- --------------------------- ALARM

do

    alarmE1,alarmE2 = 0,0;

    function alarm()
        object_color("scaner",1.0,0.0,0.0,0.79);
        object_color("l1_alert",1.0,0.0,0.0,0.79);
        object_color("l2_alert",1.0,0.0,0.0,0.79);

        alarmE1 = ai_spawn("zombie",ai_getPosition(ai_findByName("spawnPnt1")));
        clock_postPhysStep({command='ai_move(alarmE1, true)'})

        alarmE2 = ai_spawn("zombie",ai_getPosition(ai_findByName("a_spawn")));
        clock_postPhysStep({command='ai_move(alarmE2, false)'})

    end

end


-- ------------------------ EXPLOSIONS EVENT

EXP_COLOR = {r=0.88, g=0.57, b=0.15, a=0.69}
do

    local totalExplosions;
    local sx,sy = 0,0;
    local floorJoints = 0;

    function spawnEx()
        local uid = ai_findByName("stEx");
        sx,sy = ai_getPosition(uid);
        totalExplosions = 30;
        floorJoints = 0;
        camera_lock(uid,0.5,-0.5);
        ee_spawnExImpl()

        input_block(true)
        ai_stop(PROTAGONIST_UID)
        --clock_scale(0.5);
        --

        clock_schedule({command='clock_smoothScale(0.2,1000,"out",5)',startOffset=250});
        clock_schedule({func="ee_reset",startOffset=1000});
    end

    function ee_reset()
        input_block(false)
        clock_smoothScale(1.0,2500,"in",5);
        lockCamera2Char()
    end

    function ee_spawnExImpl()
        if(totalExplosions<=0) then
            return;
        end
        totalExplosions = totalExplosions - 1;
        if totalExplosions % 2 == 0 then
            floorJoints = floorJoints + 1;
            object_delete("flst" .. floorJoints);
        end
        local rad = math.random(0.8,1.2)
        local yOffset = math.random(0,0.3)
        effect_play("explosion", {x=sx,y=sy+yOffset,hw=rad,hh=rad}, EXP_COLOR, 5);
        sound_play(expid);
        sx=sx+0.6;
        clock_schedule({func="ee_spawnExImpl",startOffset=250});
    end

end
-- ---------------------- END EVENT

do

    function endExp()

        clock_scale(0.2);
        object_delete("jj1");
        object_delete("jj2");
        object_delete("jj3");
        object_delete("fff1");
        object_delete("fff2");

        local x1,y1 = ai_getPosition(ai_findByName("ff1"));
        local x2,y2 = ai_getPosition(ai_findByName("ff2"));
        local x3,y3 = ai_getPosition(ai_findByName("ff3"));

        effect_play("explosion", {x=x1,y=y1,hw=1.0,hh=1.0}, EXP_COLOR, 5);
        effect_play("explosion", {x=x2,y=y2,hw=0.8,hh=0.8}, EXP_COLOR, 5);
        effect_play("explosion", {x=x3,y=y3,hw=0.9,hh=0.9}, EXP_COLOR, 5);
        sound_play(expid);

        object_break("fe1");
        clock_smoothScale(1.0,2500,"in",5);
        camera_easePow(0);
        lockCamera2Char()
    end

    function endLvl()
        camera_lock(PROTAGONIST_UID,200,100)
        sound_stopAll();
        clock_pause(true)

        sound_play(expid);
    end

end


