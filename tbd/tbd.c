/******************************************************************************
        The Beast's Domain  Copyright (c) 1993-2000 Domain Entertainment
******************************************************************************/

#include "tbd.h"
#define VERSION "2K"

void main(int argc, char **argv);
#if 0
void interrupt (*oldhandler)(void);
void interrupt monster_clock(void);
#endif

unsigned _stklen=20000;

void main(int argc, char **argv)
{
    FILE *fp;
    char str[256],chbuf[8],*buf,*p,name[26];
    int file,x,r1,r2,ch,times_played=0,lev,maint_only=0;
    long lastrun,length,l,exp;

    xp_randomize();
    sprintf(node_dir,"%s",getenv("SBBSNODE"));
    if(node_dir[strlen(node_dir)-1]!='\\')
        strcat(node_dir,"\\");
    if(!stricmp(argv[1],"/?")) {
        if(argc);
        bprintf("\r\nThe Beast's Domain v%s/XSDK %s  Copyright 2000 Domain "
                "Entertainment\r\n",VERSION,xsdk_ver);
        bputs("\r\nUsage: TBD /(switch)\r\n");
        bputs("\r\nWhere '/(switch)' is one or more of the following:\r\n");
        bputs("\r\n      /MAINT will force a daily maintenance run (this may "
              "be used in place of");
        bputs("\r\n             the normal automatic daily maintenance).");
        bputs("\r\n      /COST=<cost>       is the cost (in credits) PER "
              "MINUTE to play the game");
        bputs("\r\n      /TIMES=<times/day> is the number of times a player "
              "can play each day\r\n");
        return; }

    for(x=1; x<argc; x++) {
        if (!strchr(argv[x],'/')) {
            strcpy(node_dir,argv[x]);
            if (node_dir[strlen(node_dir)-1]!='\\')
                strcat(node_dir,"\\"); }
        if(strstr(strupr(argv[x]),"/COST=")) {
            p=strchr(argv[x],'=');
                if(p!=NULL) cost_per_min=atoi(p+1); }
        if(strstr(strupr(argv[x]),"/TIMES=")) {
            p=strchr(argv[x],'=');
                if(p!=NULL) times_per_day=atoi(p+1); }
        if(strstr(strupr(argv[x]),"/MAINT"))
            maint_only=1;
    }

    initdata();

    if(!(fexist("TBD.MNT")) || maint_only) {
        if((file=nopen("TBD.MNT",O_WRONLY|O_CREAT))!=-1) {
            cls();
            bprintf("\r\nPLEASE WAIT: Running daily maintenance.");
#if 0
    struct date d;
    struct time t;
            unixtodos(lastrun,&d,&t);
            t.ti_hour=t.ti_min=t.ti_sec=t.ti_hund=0;
            lastrun=dostounix(&d,&t);
#endif
            lastrun=time(NULL);
            lastrun -= lastrun % 86400;
            write(file,&lastrun,4); close(file);
            perform_daily_maintenance(); } }
    if((file=nopen("TBD.MNT",O_RDWR))==-1) {
        cls(); bprintf("\r\nPLEASE WAIT: Daily maintenance is running.");
        for(x=0;x<(SQUARE/2);x++) { mswait(1000); bprintf("."); }
    } else {
        read(file,&lastrun,4);
        if(time(NULL)-lastrun>=86400) {
            cls();
            bprintf("\r\nPLEASE WAIT: Running daily maintenance.");
#if 0
    struct date d;
    struct time t;
            unixtodos(lastrun,&d,&t);
            t.ti_hour=t.ti_min=t.ti_sec=t.ti_hund=0;
            lastrun=dostounix(&d,&t);
#endif
            lastrun=time(NULL); lseek(file,0L,SEEK_SET);
            lastrun -= lastrun % 86400;
            write(file,&lastrun,4);
            perform_daily_maintenance(); }
        close(file); }
    if(maint_only) return;

    cls();
    center_wargs("\1n\1gWelcome to The Beast's Domain v%s",VERSION);
    center_wargs("\1n\1gCopyright 2000 Domain Entertainment");

    if(fexist("TBD.SYS")) printfile("TBD.SYS");

    do {
        mnehigh=GREEN|HIGH; mnelow=GREEN;
        mnemonics("\r\n\r\n~Story of The Beast's Domain\r\n");
        mnemonics("~How to play The Beast's Domain\r\n");
        if(cost_per_min)
            mnemonics("~Cost to play The Beast's Domain\r\n");
        mnemonics("~Top Ten Warriors list\r\n");
        mnemonics("~List of the Beast's Conqueror's\r\n");
        mnemonics("~Enter The Beast's Domain\r\n");
        mnemonics("~Quit back to ");
        bprintf("\1y\1h%s\1n",sys_name);
        bprintf("\r\n"); nodesync();
        bprintf("\r\n\1m\1hCommand:\1n ");
        ch=getkey(K_UPPER);
        bprintf("%c\r\n",ch);
		switch(ch) {
			case 'Q':
                bprintf("\r\n\r\n\1n\1gReturning you to \1g\1h%s\r\n\r\n"
                    "\1n",sys_name);
                return;
            case 'S':
                printfile("TBD.STY");
                break;
            case 'H':
                printfile("TBD.INS");
                break;
            case 'E':
                if(cost_per_min && (!user_cdt || cost_per_min>(long)user_cdt) &&
                   !strchr(user_exempt,'D')) {
                    bprintf("\r\n\1h\1i\1rYou don't have enough credits to "
                            "play!\1n");
                    ch=0;
                    break;
                }
                break;
            case 'C':
                if(cost_per_min)
                    bprintf("\r\n\1n\1mThe cost to play The Beast's Domain "
                            "is \1h%ld\1n\1m credits per minute.",
                            cost_per_min);
                break;
            case 'T':
                list_users();
                break;
            case 'L':
                if(!(fexist("TBDCHAMP.LST"))) {
                    bprintf("\r\n\1c\1hThe Beast Remains UNDEFEATED!");
                    break; }
                if((file=nopen("TBDCHAMP.LST",O_RDONLY))==-1) {
                    printf("Error opening champions file\r\n");
                    exit(1); }
                cls();
                center_wargs("\1c\1h������������������ķ");
                center_wargs("\1c\1h� \1yChampions of TBD \1c�");
                center_wargs("\1c\1h�������������������������������������ķ");
                center_wargs("\1c\1h� \1yPlayer Name                   Level "
                    "\1c�");
                center_wargs("\1c\1h�������������������������������������Ľ");
                while(!eof(file)) {
                    read(file,name,25);
                    read(file,&exp,4);
                    read(file,&lev,2);
                    truncsp(name);
                    sprintf(str,"\1m\1h%-25.25s     \1r%5d",name,lev);
                    center_wargs(str); }
                close(file);
                break;
		}
    } while(ch!='E');
#if 0
    atexit(restore_clock);
    oldhandler=getvect(0x1C);
    setvect(0x1C,monster_clock);
#endif
    redraw_screen=0; tpic=0;
    sprintf(str,"MESSAGE.%d",node_num);
    if(fexist(str)) unlink(str);
    sprintf(str,"DAMAGE.%d",node_num);
    if(fexist(str)) unlink(str);
    if(fexist("TBDTODAY.LOG")) {
        if((file=nopen("TBDTODAY.LOG",O_RDONLY))==-1) {
            printf("Error opening time file\r\n");
            exit(1);}
        length=filelength(file);
        if ((buf=(char *)MALLOC(length))!=NULL) {
            read(file,buf,length);
        }
        close(file);
        for (l=0;l<length;l+=25) {
            sprintf(str,"%.25s",buf+l);
            truncsp(str);
            if (!stricmp(str,user_name))
                times_played++;
        }
        FREE(buf);
    }
    if(times_played>=times_per_day && times_per_day && !SYSOP) {
        bprintf("\1r\1h\r\nYou can only play %d times each day!\r\n",
            times_per_day);
        pause(); bprintf("\1n"); return; }

    if((file=nopen("TBDTODAY.LOG",O_WRONLY|O_APPEND|O_CREAT))==-1) {
        printf("Error opening time file\r\n");
        exit(1);}
    write(file,user_name,25);
    close(file);

    if(!(fexist("TBDCHAR.POS"))) {
        if((chfile=open("TBDCHAR.POS",O_RDWR|O_DENYNONE|O_CREAT|O_BINARY,
            S_IWRITE|S_IREAD))==-1) {
            printf("Error opening character position file!\r\n");
            exit(1); }
        for(x=0;x<sys_nodes;x++)
            write(chfile,"\0\0\0\0\0\0\0\0",8);
        close(chfile); }
    if((chfile=open("TBDCHAR.POS",O_RDWR|O_DENYNONE|O_CREAT|O_BINARY,
            S_IWRITE|S_IREAD))==-1) {
            printf("Error opening character position file!\r\n");
            exit(1); }
    if((rmfile=open("TBDOBJ.DAB",O_RDWR|O_DENYNONE|O_CREAT|O_BINARY,
        S_IWRITE|S_IREAD))==-1) {
        printf("Error opening rooms file! %d\r\n",errno);
        exit(1);
    }
    memset(user.space,'\0',strlen(user.space));
    if(!read_user(0)) {
        read_user(1); user.mapx=SQUARE/2; user.mapy=0; user.mapz=3;
        user.roomy=0; user.roomx=5; user.health=40; user.max_health=40;
        user.exp=0L; user.status=1; user.gold=0; user.level=0; user.armor=0;
        user.weapon=0; user.left_game=time(NULL)-90; user.ring=0; user.key=0;
        user.compass=0; for(x=0;x<6;x++) user.item[x]=0;
        if(fexist("TBD.CFG")) {
            if((file=nopen("TBD.CFG",O_RDONLY))!=-1) {
                if((fp=fdopen(file,"rt"))!=NULL) {
                    fgets(str,81,fp);
                    user.health=atoi(str);
                    fgets(str,81,fp);
                    user.weapon=atoi(str);
                    fgets(str,81,fp);
                    user.armor=atoi(str);
                    fgets(str,81,fp);
                    user.gold=atoi(str);
                    fclose(fp); } } }
        write_user(); }
    read_map_level(user.mapz);
    r1=user.roomx; r2=user.roomy;
    get_room_objects(user.mapz,(user.mapy*SQUARE)+user.mapx,0);
    cls();
    while(initial_inway(user.mapx,user.mapy,user.mapz,r1,r2)) {
        r1=rand()%10; r2=rand()%4; }
    movement(user.mapx,user.mapy,user.mapz,r1,r2);
}
/******************************************************************************
 This is the main loop of the game, this is where all commands are interpreted
******************************************************************************/
void movement(int sx,int sy,int sz,int sgx,int sgy)
{
    char ch,str[256],chbuf[8],active[251][2];
    int gx=sgx,gy=sgy,x=sx,y=sy,z=sz,lx=gx,ly=gy,n,ox[251][2],oy[251][2],
        oz[251][2],ogx[251][2],ogy[251][2],file,firstime=1,check,menu=0,
        dead=-1,temp,count,warn,numnodes=0,oldnumnodes=0;
    uchar hitnode,olevel,damage;
    long timeleftmin;
    time_t strength_timer,invis_timer,now,timeout,health_timer;
    node_t node;

    clock_tick=invisible=strong=0; clock_tick2=40;
    printfile("TBD.MNU"); game_commands(0,-1);
    health_timer=user.left_game;
    timeleftmin=(timeleft-(time(NULL)-starttime))/60L;
    bprintf("\x1b[2;41H\1h\1y%6ld",timeleftmin);
    bprintf("\x1b[3;41H\1h\1y%6d",(int)user.level);
    if((user.health*100)/user.max_health<=25) bprintf("\1h\1r\1i");
    else bprintf("\1h\1y");
    bprintf("\x1b[4;41H%6d\1n",user.health);
    if(user.key) bprintf("\x1b[13;7H\1m\1h[\1rKEY\1m]");
    drawpos(x,y,z);
    bprintf("\x1b[%d;%dH\1y\1h\2",gy+7,gx+35);
    write_movement(2,x,y,z,gx,gy);
    if(rmobj[(gy*11)+gx].item) {
        if(object[rmobj[(gy*11)+gx].item].type==GOLD)
            status_message(2,"\1y\1h%d gold pieces.",rmobj[(gy*11)+gx].val*10);
        else {
            if(in_shop && object[rmobj[(gy*11)+gx].item].type!=TRADINGPOST)
                status_message(2,"\1g\1h%s - %d gold pieces",
                               object[rmobj[(gy*11)+gx].item].name,
                               object[rmobj[(gy*11)+gx].item].value);
            else
                status_message(2,"\1g\1h%s",
                               object[rmobj[(gy*11)+gx].item].name); } }

    timeout=time(NULL);                         /* Set timer on entry point */
    do {
        ++clock_tick2;
        now=time(NULL);                         /* Increment timer in loop  */
        if(clock_tick%6==0 || clock_tick%6==1)
            comp_room_objects(z,(y*SQUARE)+x);
        if(clock_tick==9 || clock_tick>=18) {
            if(!invisible)
                monster_check(gx,gy);           /* Check for monster attacks */
            sprintf(str,"MESSAGE.%d",node_num);
            if (flength(str)>0) {               /* Checks for new player msg */
                do {
                    read_player_message();
                    mswait(100);
                } while (flength(str)>0); }
            if(clock_tick>=18) clock_tick=0;
        }
        if(wherey()!=gy+7 || wherex()!=gx+36)   /* Replace cursor if moved */
            bprintf("\x1b[%d;%dH",gy+7,gx+36);
        if((ch=toupper(inkey(0)))!=0) {
            warn=0; timeout=time(NULL);         /* Reset timer on key press */
            comp_room_objects(z,(y*SQUARE)+x);  /* Compare room objects */
            bprintf("\x1b[%d;%dH",gy+7,gx+36);
        switch(ch) {
            case 'S':                           /* Send Message */
                send_message(oldnumnodes);
                break;
            case '4':                           /* Move West */
            case 'A':
                check=inway(x,y,z,gx-1,gy);
                if(weapon_ready) { tpic=LEFT;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gx>0 && !check) {
                    gx--; bprintf("\b \b\b\1y\1h%c",tpic); }
                else if(gx==0 && gy==2 && (map[z][y][x]&W_DOOR)) {
                    if(x>0) {
                        if(inway(x-1,y,z,10,gy)) {
                            drawpos(x-1,y,z); drawpos(x,y,z);
                            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                        } else {
                            x--; gx=10; drawpos(x,y,z);
                            bprintf("\x1b[9;45H\1y\1h%c",tpic); }
                    } else if(x==0) {
                        if(z==2 || z==3) {
                            if(inway(SQUARE-1,y,z+1,10,gy)) {
                                drawpos(SQUARE-1,y,z+1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                x=SQUARE-1; gx=10; user.mapz++; }
                        } else if(z==7) {
                            if(inway(SQUARE-1,y,z-1,10,gy)) {
                                drawpos(SQUARE-1,y,z-1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                x=SQUARE-1; gx=10; user.mapz--; }
                        }
                    }
                } else if(gx==0) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '6':                           /* Move East */
            case 'D':
                check=inway(x,y,z,gx+1,gy);
                if(weapon_ready) { tpic=RIGHT;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gx<10 && !check) {
                    gx++; bprintf("\b \b\x1b[C\1y\1h%c",tpic); }
                else if(gx==10 && gy==2 && (map[z][y][x]&E_DOOR)) {
                    if(x<SQUARE-1) {
                        if(inway(x+1,y,z,0,gy)) {
                            drawpos(x+1,y,z); drawpos(x,y,z);
                            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                        } else {
                            x++; gx=0; drawpos(x,y,z);
                            bprintf("\x1b[9;35H\1y\1h%c",tpic); }
                    } else if(x==SQUARE-1) {
                        if(z==3 || z==4) {
                            if(inway(0,y,z-1,10,gy)) {
                                drawpos(0,y,z-1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                x=0; gx=0; user.mapz--; }
                        } else if(z==6) {
                            if(inway(0,y,z+1,10,gy)) {
                                drawpos(0,y,z+1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                x=0; gx=0; user.mapz++; }
                        }
                    }
                } else if(gx==10) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '2':                           /* Move South */
            case 'X':
                check=inway(x,y,z,gx,gy+1);
                if(weapon_ready) { tpic=DOWN;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy<4 && !check) {
                    gy++; bprintf("\b \b\x1b[B\1y\1h%c",tpic); }
                else if((gy==4 && gx==5 && (map[z][y][x]&S_DOOR)) ||
                       (gy==4 && gx>2 && gx<8 && x==(SQUARE/2) &&
                       y==SQUARE-1)) {
                    if(y<SQUARE-1) {
                        if(inway(x,y+1,z,gx,0)) {
                            drawpos(x,y+1,z); drawpos(x,y,z);
                            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                        } else {
                            y++; gy=0; drawpos(x,y,z);
                            bprintf("\x1b[7;40H\1y\1h%c",tpic); }
                    } else if(y==SQUARE-1) {
                        if(z==4 || z==5) {
                            if(inway(x,0,z+1,10,gy)) {
                                drawpos(x,0,z+1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                y=0; gy=0; user.mapz++; }
                        } else if(z==1 || z==2 || z==8) {
                            if(inway(x,0,z-1,10,gy)) {
                                drawpos(x,0,z-1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                y=0; gy=0; user.mapz--; }
                        }
                    }
                } else if(gy==4) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '8':                           /* Move North */
            case 'W':
                check=inway(x,y,z,gx,gy-1);
                if(weapon_ready) { tpic=UP;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy==0 && x==(SQUARE/2) && y==0 && z==7 && !user.key) {
                    status_message(0,"\1r\1hThe Killer Mantis Holds The Key!");
                    break; }
                if(gy>0 && !check) {
                    gy--; bprintf("\b \b\x1b[A\1y\1h%c",tpic); }
                else if((gy==0 && gx==5 && (map[z][y][x]&N_DOOR)) ||
                        (gy==0 && gx>2 && gx<8 && x==(SQUARE/2) && y==0)) {
                    if(y>0) {
                        if(inway(x,y-1,z,gx,4)) {
                            drawpos(x,y-1,z); drawpos(x,y,z);
                            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                        } else {
                            y--; gy=4; drawpos(x,y,z);
                            bprintf("\x1b[11;40H\1y\1h%c",tpic);}
                    } else if(y==0) {
                        if(z==0 || z==1 || z==7) {
                            if(inway(x,SQUARE-1,z+1,10,gy)) {
                                drawpos(x,SQUARE-1,z+1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                y=SQUARE-1; gy=4; user.mapz++; }
                        } else if(z==5 || z==6) {
                            if(inway(x,SQUARE-1,z-1,10,gy)) {
                                drawpos(x,SQUARE-1,z-1); drawpos(x,y,z);
                                bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                            } else {
                                y=SQUARE-1; gy=4; user.mapz--; }
                        }
                    }
                } else if(gy==0) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '7':                           /* Move NorthWest */
            case 'Q':
                check=inway(x,y,z,gx-1,gy-1);
                if(weapon_ready) { tpic=UP;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy>0 && gx>0 && !check) {
                    gy--; gx--; bprintf("\b \b\x1b[D\x1b[A\1y\1h%c",
                                            tpic); }
                else if(gy==0 && gx==0 && (map[z][y][x]&NW_DOOR)
                         && y>0 && x>0) {
                    if(inway(x-1,y-1,z,10,4)) {
                        drawpos(x-1,y-1,z); drawpos(x,y,z);
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,
                                tpic);
                    } else {
                        y--; x--; gx=10; gy=4; drawpos(x,y,z);
                        bprintf("\x1b[11;45H\1y\1h%c",tpic); }
                }
                else if(gy==0 || gx==0) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '9':                           /* Move NorthEast */
            case 'E':
                check=inway(x,y,z,gx+1,gy-1);
                if(weapon_ready) { tpic=UP;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy>0 && gx<10 && !check) {
                    gy--; gx++; bprintf("\b \b\x1b[C\x1b[A\1y\1h%c",
                                            tpic); }
                else if(gy==0 && gx==10 && (map[z][y][x]&NE_DOOR)
                         && y>0 && x<SQUARE-1) {
                    if(inway(x+1,y-1,z,0,4)) {
                        drawpos(x+1,y-1,z); drawpos(x,y,z);
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,
                                tpic);
                    } else {
                        y--; x++; gx=0; gy=4; drawpos(x,y,z);
                        bprintf("\x1b[11;35H\1y\1h%c",tpic); }
                }
                else if(gy==0 || gx==10) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '1':                           /* Move SouthWest */
            case 'Z':
                check=inway(x,y,z,gx-1,gy+1);
                if(weapon_ready) { tpic=DOWN;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy<4 && gx>0 && !check) {
                    gy++; gx--; bprintf("\b \b\x1b[D\x1b[B\1y\1h%c",
                                            tpic); }
                else if(gy==4 && gx==0 && (map[z][y][x]&SW_DOOR)
                         && y<SQUARE-1 && x>0) {
                    if(inway(x-1,y+1,z,10,0)) {
                        drawpos(x-1,y+1,z); drawpos(x,y,z);
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,
                                tpic);
                    } else {
                        y++; x--; gx=10; gy=0; drawpos(x,y,z);
                        bprintf("\x1b[7;45H\1y\1h%c",tpic); }
                }
                else if(gy==4 || gx==0) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '3':                           /* Move SouthEast */
            case 'C':
                check=inway(x,y,z,gx+1,gy+1);
                if(weapon_ready) { tpic=DOWN;
                    if(check)
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                }
                if(gy<4 && gx<10 && !check) {
                    gy++; gx++; bprintf("\b \b\x1b[C\x1b[B\1y\1h%c",
                                            tpic); }
                else if(gy==4 && gx==10 && (map[z][y][x]&SE_DOOR)
                         && y<SQUARE-1 && x<SQUARE-1) {
                    if(inway(x+1,y+1,z,0,0)) {
                        drawpos(x+1,y+1,z); drawpos(x,y,z);
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,
                                tpic);
                    } else {
                        y++; x++; gx=0; gy=0; drawpos(x,y,z);
                        bprintf("\x1b[7;35H\1y\1h%c",tpic); }
                }
                else if(gy==4 || gx==10) { bprintf("\b\1y\1h%c",tpic); }
                break;
            case '5':                           /* Use Special Item */
            case 'U':
                if(menu!=1) game_commands(1,-1);
                count=use_item();
                if(count==7 || count==8) count=rand()%7+1;
                if(count==1) {                 /* Warp */
                    if(warp_to_other()) {
                        x=user.mapx; y=user.mapy; z=user.mapz;
                        gx=user.roomx; gy=user.roomy;
                        read_map_level(user.mapz);
                        get_room_objects(z,(y*SQUARE)+x,0);
                        while(initial_inway(user.mapx,user.mapy,user.mapz,
                            gx,gy)) {
                            gx=rand()%10; gy=rand()%4; }
                        user.roomx=gx; user.roomy=gy; drawpos(x,y,z);
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                        status_message(1,"\1m\1hYou have been warped!");
                    } else
                        status_message(1,"\1r\1hNobody else in the land!"); }
                if(count==2) {                 /* Invisibility 60 seconds */
                    bprintf("\x1b[17;30H\1m\1h[\1rINVISIBLE\1m]");
                    invis_timer=time(NULL);
                    invisible=1; }
                if(count==3) {                 /* Potion of Healing */
                    temp=(rand()%(user.max_health/3))+1; user.health+=temp;
                    status_message(1,"\1c\1hHealing good for %d points!",
                        temp);
                    if(user.health>user.max_health)
                        user.health=user.max_health;
                    bprintf("\x1b[4;41H\1y\1h%6d",user.health); }
                if(count==4) {                 /* Scroll of Xtra Heal */
                    temp=(rand()%(user.max_health/2))+5; user.health+=temp;
                    status_message(1,"\1c\1hExtra Heal good for %d "
                        "points!",temp);
                    if(user.health>user.max_health)
                        user.health=user.max_health;
                    bprintf("\x1b[4;41H\1y\1h%6d",user.health); }
                if(count==5) {
                    user.health=user.max_health;
                    bprintf("\x1b[4;41H\1y\1h%6d",user.health);
                    status_message(1,"\1c\1hYou feel as good as new!"); }
                if(count==6) {
                    bprintf("\x1b[17;43H\1m\1h[\1ySTRONG\1m]");
                    strength_timer=time(NULL);
                    strong=1; }
                if(count==7) {                 /* Damage from Mystery */
                    temp=(rand()%10)+1; user.health-=temp;
                    status_message(1,"\1r\1hYou take %d points of damage!\7",
                        temp);
                    if((user.health*100)/user.max_health<=25)
                        bprintf("\1h\1r\1i");
                    else bprintf("\1h\1y");
                    bprintf("\x1b[4;41H%6d\1n",user.health);
                }
                if(count==8) {                 /* Damage from Mystery */
                    temp=(rand()%10)+5; user.health-=temp;
                    status_message(1,"\1r\1hYou take %d points of damage!\7",
                        temp);
                    if((user.health*100)/user.max_health<=25)
                        bprintf("\1h\1r\1i");
                    else bprintf("\1h\1y");
                    bprintf("\x1b[4;41H%6d\1n",user.health);
                }
                if(count==9) {
                    temp=(rand()%10)+1; user.health+=temp;
                    status_message(1,"\1c\1hYou heal %d points!",temp);
                    if((user.health*100)/user.max_health<=25)
                        bprintf("\1h\1r\1i");
                    else bprintf("\1h\1y");
                    bprintf("\x1b[4;41H%6d\1n",user.health);
                }
                if(count || menu!=1) game_commands(menu,-1);
                break;
            case '.':                           /* Inventory Toggle */
            case 'I':
                if(menu) { menu=0; game_commands(menu,-1); }
                else { menu=1; game_commands(menu,-1); }
                break;
            case '0':                           /* Un/Ready Weapon */
            case 'R':
                if(in_shop) {
                    status_message(0,"\1r\1hYou cannot use weapons in a shop!");
                } else if(weapon_ready) {
                    tpic=2; bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                    weapon_ready=0; }
                else weapon_ready=1;
                break;
            case '+':
                temp=-1;
                if(weapon_ready) {
                    status_message(1,"\1r\1hYou can't pick up items or use "
                        "doorways/stairs with your weapon out"); }
                else if(rmobj[(gy*11)+gx].item) {
                    if((temp=pickup_object(gx,gy,z,(y*SQUARE)+x))!=0) {
                        if(temp==255) { game_winner(); return; }
                        if(menu==1) game_commands(menu,temp); }
                    write_user(); }
                break;
            case '-':
                temp=-1;
                if(weapon_ready) {
                    status_message(1,"\1r\1hYou cannot drop items with your"
                        " weapon readied!"); }
                else if(!rmobj[(gy*11)+gx].item) {
                    if(menu!=1) game_commands(1,-1);
                    temp=drop_object(gx,gy,z,(y*SQUARE)+x);
                    if(menu==1) game_commands(menu,temp);
                    write_user(); }
                else
                    status_message(2,"\1r\1hNo room to drop anything here!");
                if(menu!=1) game_commands(menu,-1);
                break;
            case 18:                            /* ^R Redraw Screen */
                redraw_screen=1;
                break;
            case 'T':                           /* Terminate Session */
                user.mapx=x; user.mapy=y; user.roomx=gx; user.roomy=gy;
                bprintf("\1n");
                return;
        }
        if(invisible) write_movement(32,x,y,z,gx,gy);
        else write_movement(tpic,x,y,z,gx,gy);
        if(user.mapx!=x) user.mapx=x; if(user.mapy!=y) user.mapy=y;
        if(in_shop && weapon_ready) {          /* put weapon away if drawn */
            status_message(0,"\1r\1hAll weapons must be put away while you"
                           " are within shops!");
            weapon_ready=0; tpic=2;
            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
        }
        if(rmobj[(ly*11)+lx].item && (lx!=gx || ly!=gy)) {
            attr(object[rmobj[(ly*11)+lx].item].color);
            bprintf("\x1b[%d;%dH%c",ly+7,lx+35,
                    object[rmobj[(ly*11)+lx].item].symbol);
        }
        if(rmobj[(gy*11)+gx].item && (lx!=gx || ly!=gy)) {
            if(object[rmobj[(gy*11)+gx].item].type==GOLD)
                status_message(2,"\1y\1h%d gold pieces.",
                               rmobj[(gy*11)+gx].val*10);
            else {
                if(in_shop && object[rmobj[(gy*11)+gx].item].type!=TRADINGPOST)
                    status_message(2,"\1g\1h%s - %d gold pieces",
                                   object[rmobj[(gy*11)+gx].item].name,
                                   object[rmobj[(gy*11)+gx].item].value);
                else
                    status_message(2,"\1g\1h%s",
                                   object[rmobj[(gy*11)+gx].item].name);
            }
        }
        lx=gx; ly=gy;
        }
        if((time(NULL)-starttime)>timeleft) {    /* Ran out of time */
            status_message(0,"\1r\1hTimes up!\1n");
            user.mapx=x; user.mapy=y; user.roomx=gx; user.roomy=gy;
            return;
        }
        if(cost_per_min && total_cost>=(long)user_cdt && !strchr(user_exempt,'D')) {
            status_message(0,"\1r\1hYou're out of credits!");
            user.mapx=x; user.mapy=y; user.roomx=gx; user.roomy=gy;
            return;
        }
        if(z!=user.mapz) {                     /* update dungeon level */
            z=user.mapz; read_map_level(z); drawpos(x,y,z);
            bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic); }

        if(invisible) {
            if((time(NULL)-invis_timer)>30) {
                bprintf("\x1b[17;30H\1m\1h�����������");
                invisible=0; } }
        if(strong) {
            if((time(NULL)-strength_timer)>30) {
                bprintf("\x1b[17;43H\1m\1h��������");
                strong=0; } }
        /* Update the top of screen information every 60 seconds */
        if(timeleftmin!=(timeleft-(time(NULL)-starttime))/60L) {
            if(cost_per_min>0L && !strchr(user_exempt,'D')) {
                total_cost+=cost_per_min;   /* subtract credits for 1 minute */
                moduser();                  /* and write amount to the file  */
            }
            timeleftmin=(timeleft-(time(NULL)-starttime))/60L;
            bprintf("\x1b[2;41H\1h\1y%6ld",timeleftmin);
            bprintf("\x1b[3;41H\1h\1y%6d",(int)user.level);
            if((user.health*100)/user.max_health<=25) bprintf("\1h\1r\1i");
            else bprintf("\1h\1y");
            bprintf("\x1b[4;41H%6d\1n",user.health); }

        if(user.max_health>((int)(user.level)*10)+40)
            user.max_health=((int)(user.level)*10)+40;
        if(user.health>user.max_health)
            user.health=user.max_health;
        if(user.health<user.max_health) {
            if((time(NULL)-health_timer)>=6) {
                user.health+=(((int)(user.level)/5)+1)*
                    ((time(NULL)-health_timer)/6);
                if(user.health>user.max_health) user.health=user.max_health;
                health_timer=time(NULL);
                if((user.health*100)/user.max_health<=25) bprintf("\1h\1r\1i");
                else bprintf("\1h\1y");
                bprintf("\x1b[4;41H%6d\1n",user.health);
            }
        }
        if(user.level>(uchar)(user.exp/200L)) user.level=(uchar)(user.exp/200L);
        if(user.level<(uchar)(user.exp/200L)) {
            user.level=(uchar)(user.exp/200L);
            user.max_health=((int)(user.level)*10)+40; user.health+=10;
            bprintf("\x1b[3;41H\1y\1h%6d",(int)user.level);
            if((user.health*100)/user.max_health<=25) bprintf("\1h\1r\1i");
            else bprintf("\1h\1y");
            bprintf("\x1b[4;41H%6d\1n",user.health);
        }

        if(tpic!=FACE && !weapon_ready) tpic=FACE;
        lncntr=0;
        mswait(20);
        nodesync();
        sprintf(str,"DAMAGE.%d",node_num);
        if(flength(str)>0) {
            if((file=nopen(str,O_RDONLY))!=-1) {
                health_timer=time(NULL);
                read(file,&hitnode,1); read(file,&damage,1);
                read(file,&olevel,1); close(file);
                if((file=nopen(str,O_WRONLY|O_TRUNC))!=-1)
                    close(file);
                if(damage==255) {
                    getnodedat(hitnode,&node,0);
                    status_message(1,"\1c\1hYou killed %s!",
                        username(node.useron));
                    if(olevel>user.level) user.exp+=
                        (long)(olevel-user.level)*10L;
                    /* Changed, don't give exp points if monster is >= 5 levels
                       lower than player */
                    else if((user.level-5)<=olevel) user.exp+=(long)olevel;
                } else {
                    if(damage>0) {
                        bprintf("\7");
                        if(user.health<(int)damage) user.health=0;
                        else user.health-=(int)damage;
                        if(!hitnode)
                            status_message(0,"\1r\1hA monster HIT you for %d "
                                "damage!",(int)damage);
                        else if(hitnode>0) {
                            lseek(chfile,(long)(n*8),SEEK_SET); temp=0;
                            while(read(chfile,&chbuf[0],1)==-1 && temp++<100);
                            getnodedat(hitnode,&node,0);
                            status_message(0,"\1r\1h%s hit you for %d damage!",
                                (chbuf[0]==32) ? "Something" :
                                username(node.useron),(int)damage);
                        }
                     } else {
                        if(!hitnode)
                            status_message(0,"\1m\1hA monster swung at you and "
                                           "missed!");
                        else if(hitnode>0) {
                            lseek(chfile,(long)(n*8),SEEK_SET); temp=0;
                            while(read(chfile,&chbuf[0],1)==-1 && temp++<100);
                            getnodedat(hitnode,&node,0);
                            status_message(0,"\1m\1h%s swung at you and "
                                "missed!",(chbuf[0]==32) ? "Something" :
                                username(node.useron));
                        }
                    }
                    if((user.health*100)/user.max_health<=25)
                        bprintf("\1h\1r\1i");
                    else bprintf("\1h\1y");
                    bprintf("\x1b[4;41H%6d\1n",(user.health>0) ?
                        user.health : 0);
                }
            }
        }
        if(user.health<=0) {                    /* User has been killed! */
            if(user.ring==NUM_SPECIAL+6) {      /* Ring of Resurrection  */
                write_movement(88,x,y,z,gx,gy);
                status_message(0,"\1r\1h\1iAAAAAHHHHHHHHH!!!!  You DIED!");
                status_message(1,"\1n\1c\1hBut lucky for you, you are being "
                    "resurrected!");
                status_message(2,""); bprintf("\x1b[16;7H");
                user.ring=0; user.health=user.max_health; write_user();
                mswait(1000);
                status_message(2,"\1r\1h[Press ENTER to Continue Play]\1n");
                while(getch()!=13 && (com_port && ((*msr)&DCD)));
                if(menu) game_commands(menu,-1);
                status_message(0,""); status_message(2,"");
                status_message(3,"");
                write_movement(2,x,y,z,gx,gy);
            } else {
                write_movement(88,x,y,z,gx,gy);
                scatter_items(z,(y*SQUARE)+x);
                status_message(0,"\1r\1h\1iAAAAAHHHHHHHHH!!!!  You're DEAD!");
                status_message(1,"\1n\1c\1hBetter luck next time, oh not so "
                    "worthy one!");
                status_message(2,""); bprintf("\x1b[16;7H");
                user.status=0; user.exp=0L; user.level=0; write_user();
                chbuf[0]=0; chbuf[1]=chbuf[2]=chbuf[3]=chbuf[4]=chbuf[5]=
                chbuf[6]=chbuf[7]=-1; temp=0;
                lseek(chfile,(long)(node_num-1)*8L,SEEK_SET);
                while(lock(chfile,(long)(node_num-1)*8L,8) && temp++<100);
                write(chfile,chbuf,8);
                unlock(chfile,(long)(node_num-1)*8L,8);
                if(hitnode && hitnode<=sys_nodes) {
                    sprintf(str,"DAMAGE.%d",hitnode);
                    if((file=nopen(str,O_WRONLY|O_CREAT))!=-1) {
                        write(file,&node_num,1);
                        temp=-1; write(file,&temp,1);
                        write(file,&user.level,1);
                        close(file);
                    }
                }
                mswait(1000);
                status_message(2,"\1r\1h[Press ENTER]\1n");
                while(getch()!=13 && (com_port && ((*msr)&DCD)));
                return;
            }
        }
        if(redraw_screen==1) {                 /* Redraw the screen if 1 */
            printfile("TBD.MNU"); game_commands(menu,-1);
            drawpos(x,y,z);
            timeleftmin=(timeleft-(time(NULL)-starttime))/60L;
            bprintf("\x1b[2;41H\1h\1y%6ld",timeleftmin);
            bprintf("\x1b[3;41H\1h\1y%6d",(int)user.level);
            if((user.health*100)/user.max_health<=25) bprintf("\1h\1r\1i");
            else bprintf("\1h\1y");
            bprintf("\x1b[4;41H%6d\1n",user.health);
            bprintf("\x1b[%d;%dH\1h\1y%c",gy+7,gx+35,tpic);
            if(invisible) bprintf("\x1b[17;30H\1m\1h[\1rINVISIBLE\1m]");
            if(strong) bprintf("\x1b[17;43H\1m\1h[\1ySTRONG\1m]");
            if(user.key) bprintf("\x1b[13;7H\1m\1h[\1rKEY\1m]");
            redraw_screen=0; }

        numnodes=0; lseek(chfile,0L,SEEK_SET);
        for(n=0;n<sys_nodes;n++) {
            count=0;
            if(n==node_num-1) lseek(chfile,8L,SEEK_CUR);
            if(n!=node_num-1) {
                while(read(chfile,chbuf,8)==-1 && count++<500);
                if(count>=500) lseek(chfile,8L,SEEK_CUR);
                active[n][0]=chbuf[0];
                ox[n][0]=chbuf[1];
                oy[n][0]=chbuf[2];
                oz[n][0]=chbuf[3];
                ogx[n][0]=chbuf[4];
                ogy[n][0]=chbuf[5];

                if(active[n][0] && numnodes<18 && clock_tick2>=40) {
                    ++numnodes; getnodedat((n+1),&node,0);
                    bprintf("\x1b[%d;%dH\1c\1h%.2d:%-8.8s ",
                        (22+(numnodes/7)),
                        ((numnodes-1)*13),(n+1),username(node.useron));
                    if(!oldnumnodes) oldnumnodes=numnodes; }
                if(!active[n][0] && dead==n) dead=-1;
                if(active[n][0]==88 && dead!=n) {
                    dead=n; get_room_objects(z,(y*SQUARE+x),1);
                    active[n][0]=0;
                } else if(active[n][0] && dead==n) { active[n][0]=0; }
                /***************************************************/
                /* If someone has just entered the room, draw them */
                /***************************************************/
                if(active[n][0]!=active[n][1] && ox[n][0]==x && oy[n][0]==y
                   && oz[n][0]==z) {
                    attr((n+1)&0x1f);
                    bprintf("\x1b[%d;%dH%c",ogy[n][0]+7,ogx[n][0]+35,
                        active[n][0]); }
                /********************************************/
                /* Erase the other guy if he left the room! */
                /********************************************/
                if(ox[n][1]==x && oy[n][1]==y && oz[n][1]==z &&
                  (ox[n][0]!=x || oy[n][0]!=y || oz[n][0]!=z)) {
                    if(rmobj[(ogy[n][1]*11)+ogx[n][1]].item) {
                        attr(object[rmobj[(ogy[n][1]*11)+ogx[n][1]].item].
                             color);
                        bprintf("\x1b[%d;%dH%c",ogy[n][1]+7,ogx[n][1]+35,
                                object[rmobj[(ogy[n][1]*11)+ogx[n][1]].item].
                                symbol);
                    } else
                        bprintf("\x1b[%d;%dH ",ogy[n][1]+7,ogx[n][1]+35);
                }
                /*************************************************************/
                /* If the node just read in is active, and the guy is in the */
                /* same room as this node, we want to go through the loop!   */
                /*************************************************************/
                if(active[n][0] && ox[n][0]==x && oy[n][0]==y && oz[n][0]==z) {
                    /********************************************************/
                    /* Only re-draw everyone if THIS node has changed spots */
                    /* or if one of the other nodes in this room has moved! */
                    /********************************************************/
                    if(firstime || ogx[n][0]!=ogx[n][1] ||
                        ogy[n][0]!=ogy[n][1] || active[n][0]!=active[n][1]) {
                        if(ogx[n][1]>-1 && ogy[n][1]>-1 && ogx[n][1]<80
                            && ogy[n][1]<24) {
                            check=rmobj[(ogy[n][1]*11)+ogx[n][1]].item;
                            get_single_object(oz[n][1],(oy[n][1]*SQUARE+ox[n][1]),
                               (ogy[n][1]*11+ogx[n][1]));
                            if(check) {
                                attr(object[rmobj[(ogy[n][1]*11)+
                                     ogx[n][1]].item].color);
                                bprintf("\x1b[%d;%dH%c",ogy[n][1]+7,
                                        ogx[n][1]+35,object[rmobj[(ogy[n]
                                        [1]*11)+ogx[n][1]].item].symbol);
                            } else if(!(active[n][0]==32 &&
                                      active[n][0]==active[n][1]))
                                bprintf("\x1b[%d;%dH ",ogy[n][1]+7,
                                ogx[n][1]+35);
                        }
                        if(ogx[n][0]>-1 && ogy[n][0]>-1) {
                            if(!(active[n][0]==32 &&
                                  active[n][0]==active[n][1])) {
                                attr((n+1)&0x1f);
                                bprintf("\x1b[%d;%dH%c",ogy[n][0]+7,
                                        ogx[n][0]+35,active[n][0]);
                            }
                        }
                        bprintf("\x1b[%d;%dH\1y\1h%c",gy+7,gx+35,tpic);
                    }
                }
                ox[n][1]=ox[n][0]; oy[n][1]=oy[n][0]; oz[n][1]=oz[n][0];
                active[n][1]=active[n][0];
                ogx[n][1]=ogx[n][0];
                ogy[n][1]=ogy[n][0];
            }
        }
        if(clock_tick2>=40) {
            if(oldnumnodes>numnodes) {
                for(temp=oldnumnodes;temp>=numnodes+1;temp--)
                    bprintf("\x1b[%d;%dH            ",(22+(temp/7)),
                        ((temp-1)*13)); }
            oldnumnodes=numnodes;
            clock_tick2=0; }
        firstime=0;
        if(com_port && !((*msr)&DCD)) {         /* User hung up on game! */
            user.mapx=x; user.mapy=y; user.roomx=gx; user.roomy=gy;
            bprintf("\1n");
            return;
        }
        if((now-timeout)>120 && !warn) { bprintf("\7\7\7"); warn=1; }
    } while((now-timeout)<130);                 /* 2 minutes 10 secs */
        user.mapx=x; user.mapy=y; user.roomx=gx; user.roomy=gy;
        bprintf("\1n");
}

#if 0
/******************************************************************************
 This is the clock counter that tells the monsters when to move.
******************************************************************************/
void interrupt monster_clock(void)
{
   ++clock_tick;
   oldhandler();
}

/******************************************************************************
 This routine is called upon exit to restore the clock interrupt handler and
 clean up the game.
******************************************************************************/
void restore_clock()
{
    char str[256],chbuf[8],x=0;

    setvect(0x1c,oldhandler);
    lseek(chfile,(long)(node_num-1)*8L,SEEK_SET); x=0;
    chbuf[0]=0; chbuf[1]=chbuf[2]=chbuf[3]=chbuf[4]=chbuf[5]=chbuf[6]=chbuf[7]
        =-1;
    while(lock(chfile,(long)(node_num-1)*8L,8) && x++<100);
    write(chfile,chbuf,8);
    unlock(chfile,(long)(node_num-1)*8L,8);
    close(chfile);
    if(time(NULL)-user.left_game>120) user.left_game=time(NULL);
    write_user();
    sprintf(str,"DAMAGE.%d",node_num);
    if(fexist(str)) unlink(str);
    sprintf(str,"MESSAGE.%d",node_num);
    if(fexist(str)) unlink(str); cls();
    if(!(noyes("\r\nView the Top Ten Warriors list"))) {
        list_users(); pause(); }
    bprintf("\1n");
}
#endif

/******************************************************************************
 This function reads any messages waiting for the user
******************************************************************************/
void read_player_message()
{
    char *buf,str[256];
    int file;
    ulong length;

    sprintf(str,"MESSAGE.%d",node_num);
    if((file=nopen(str,O_RDONLY))==-1) {
        bprintf("\x1b[19;0HFile not Found: %s",str);
        return; }
    length=filelength(file);
    if((buf=malloc(length+1L))==NULL) {
        close(file);
        bprintf("\x1b[19;0H\7PRINTFILE: Error allocating %lu bytes of memory "
            "for %s.",length+1L,str); return; }
    buf[read(file,buf,length)]=0;
    close(file);
    if((file=nopen(str,O_WRONLY|O_TRUNC))==-1) {
        bprintf("\x1b[19;0HFile not Found: %s",str);
        return; }
    close(file);
    bprintf("\x1b[19;0H");
    bputs(buf);
    free(buf);
    return;
}
/******************************************************************************
 This function sends messages to player number plr
******************************************************************************/
void send_message(int nodes)
{
    int file,num,n;
    char ch,fname[81],str[256],str1[256],count=0,chbuf[8];

    if(!nodes) {
        bprintf("\x1b[18;0H\x1b[K\1r\1hYou are the only player!\1n");
        return;
    }
    while(1) {
        bprintf("\x1b[18;0H\x1b[K\1b\1hTo Whom (\1cQ\1buit, \1cR\1boom, "
                "\1c#\1b\1h) [Enter=%u]: \1n",lastnodemsg);
        num=getkeys("QR",sys_nodes);
        if(num==-1) { bprintf("\x1b[18;0H\x1b[K"); return; }
        if(num&0x8000 || !num) {
            if(!num) num=lastnodemsg;
            else {
                num^=0x8000;
                lastnodemsg=num; }
            if(!lastnodemsg) return;
            if(num==node_num) {
                bprintf("\x1b[18;0H\x1b[K\1r\1hDon't send messages to yourself!"
                    "\1n");
                return;
            }
            lseek(chfile,(long)(num-1)*8L,SEEK_SET);
            while(read(chfile,chbuf,8)==-1 && count++<100);
            if(!chbuf[0]) {
                bprintf("\x1b[18;0H\x1b[K\1r\1hThat node isn't in the game!"
                    "\1n");
                return; }
            if(chbuf[0] && (num!=node_num)) {
                bprintf("\x1b[18;0H\x1b[K\1b\1hMessage:\1n");
                if(getstr(str1,64,K_LINE)) {
                    sprintf(str,"\1g\1hFrom %-8.8s: \1n\1g%-64.64s",
                        user_name,str1);
                    send_player_message(num,str);
                } break;
            } else bprintf("\x1b[18;0H\1r\1hInvalid Player Number!\r\n");
        } else if(num=='R') {
            bprintf("\x1b[18;0H\x1b[K\1b\1hMessage:\1n");
            if(getstr(str1,64,K_LINE)) {
                sprintf(str,"\1g\1hFrom %-8.8s: \1n\1g%-64.64s",
                    user_name,str1);
                lseek(chfile,0L,SEEK_SET);
                for(n=0;n<sys_nodes;n++) {
                    count=0; if(n==node_num-1) lseek(chfile,8L,SEEK_CUR);
                    if(n!=node_num-1) {
                        while(read(chfile,chbuf,8)==-1 && count++<100);
                        if(chbuf[0] && chbuf[1]==user.mapx && chbuf[2]==user.mapy
                           && chbuf[3]==user.mapz)
                        send_player_message(n+1,str); } }
                } break;
        } else if(num=='Q') break;
    }
    bprintf("\x1b[18;0H\x1b[K");
    return;
}
/******************************************************************************
 This function sends messages to player number plr
******************************************************************************/
void send_player_message(int plr, char *msg)
{
    int file;
    char fname[81];

    sprintf(fname,"MESSAGE.%d",plr);

    if((file=nopen(fname,O_WRONLY|O_APPEND|O_CREAT))==-1) {
        bputs("Couldn't open MESSAGE.xxx for WRITE");
        return;
    }
    write(file,msg,strlen(msg));
    close(file);
}
/******************************************************************************
 This function is what re-stocks the dungeon each day.
******************************************************************************/
void perform_daily_maintenance()
{
    FILE *stream;
    int x,y,z,monster=0,tradingpost=0,fountain=0,spot=0,ch,val,rnd,num;
    long room,level;

    if((stream=fopen("TBDOBJ.DAB","r+b"))==NULL) {
        printf("Error opening room file! %d\r\n",errno);
        exit(1); }

    for(level=0L;level<LEVELS;level++) {
        bprintf(".");
        for(room=0L;room<(SQUARE*SQUARE); room++) {
            monster=tradingpost=fountain=0;
            fseek(stream,(long)(level*SQUARE*SQUARE*110L)+(long)(room*110L),
                SEEK_SET);
            fread(rmobj,110,1,stream);
            for(x=0;x<55;x++) {
                if(rmobj[x].item) {
                    if(rmobj[x].item==NUM_BLOCKAGE+4) {
                        fountain++; }
                    if(object[rmobj[x].item].type==TRADINGPOST) {
                        tradingpost++; }
                    if(object[rmobj[x].item].type==MONSTER) {
                        monster++;
                        if(rmobj[x].val<(object[rmobj[x].item].misc*10)+10) {
                            rmobj[x].val=(object[rmobj[x].item].misc*10)+10;
                            put_single_object(level,room,x); } }
                }
            }
            if(tradingpost) {
                fseek(stream,-108L,SEEK_CUR);
                if(rmobj[0].item==NUM_TRADINGPOST) {
                    for (y=0;y<3;y++) {
                        for(z=0;z<18;z++) {
                            if(level<5 && level>1) ch=y+NUM_WEAPON+1;
                            else ch=y+4+NUM_WEAPON; val=0;
                            fwrite(&ch,1,1,stream);
                            fwrite(&val,1,1,stream);
                        }
                    }
                } else if(rmobj[0].item==NUM_TRADINGPOST+1) {
                    for (y=0;y<3;y++) {
                        for(z=0;z<18;z++) {
                            if(level<5 && level>1) ch=y+NUM_ARMOR;
                            else ch=y+3+NUM_ARMOR; val=0;
                            fwrite(&ch,1,1,stream);
                            fwrite(&val,1,1,stream);
                        }
                    }
                } else if(rmobj[0].item==NUM_TRADINGPOST+2) {
                    for (y=0;y<9;y++) {
                        for(z=0;z<6;z++) {
                            ch=y+NUM_SPECIAL; val=0;
                            fwrite(&ch,1,1,stream);
                            fwrite(&val,1,1,stream);
                        }
                    }
                }
            }
            if(!monster && !tradingpost && !fountain && (rand()%4)) {
                if(level<8) rnd=rand()%1+1;
                else if(level==8) rnd=rand()%2+1;
                else if(level==9) rnd=rand()%3+1;
                for(num=0;num<rnd;num++) {
                    spot=find_empty();
                    if(level<5 && level>1) rmobj[spot].item=NUM_MONSTER+
                        rand()%7;
                    else if(level==1 || level==5)
                        rmobj[spot].item=NUM_MONSTER+7+rand()%7;
                    else rmobj[spot].item=NUM_MONSTER+10+rand()%12;
                    rmobj[spot].val=(object[rmobj[spot].item].misc*10)+10;
                    put_single_object(level,room,spot); } }
        }
    }
    fclose(stream);
    unlink("TBDTODAY.LOG");
}
/******************************************************************************
 This function is for displaying the winner message, and updating the winner
 list (if anyone actually ever wins).
******************************************************************************/
void game_winner()
{
    char chbuf[8];
    int file,temp;

    chbuf[0]=0; chbuf[1]=chbuf[2]=chbuf[3]=chbuf[4]=chbuf[5]=
    chbuf[6]=chbuf[7]=-1; temp=0;
    lseek(chfile,(long)(node_num-1)*8L,SEEK_SET);
    while(lock(chfile,(long)(node_num-1)*8L,8) && temp++<100);
    write(chfile,chbuf,8);
    unlock(chfile,(long)(node_num-1)*8L,8);

    if((file=nopen("TBDCHAMP.LST",O_WRONLY|O_APPEND|O_CREAT))==-1) {
        printf("Error opening champion file\r\n");
        exit(1);}
    write(file,user_name,25);
    write(file,&user.exp,4);
    write(file,&user.level,2);
    close(file);

    user.status=0; user.exp=0L; user.level=0; write_user();

    printfile("TBDWIN.ANS");
    pause();
}
/***************************
 Writes the MODUSER.DAT file
****************************/
void moduser()
{
    char str[256];
    FILE *fp;

    sprintf(str,"%sMODUSER.DAT",node_dir);
    if((fp=fopen(str,"wt"))==NULL) {
        printf("Can't open %s\r\n",str);
        exit(1); }
    fprintf(fp,"-%ld\r\n",total_cost);
    fclose(fp);
}
