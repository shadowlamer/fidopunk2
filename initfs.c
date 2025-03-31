#include "fs.h"

void init_fs() {
    int root;
    int home;
    int guest;
    int mikhailov;
    int echo;
    int journal;
    int _secret;
    int kozlov;
    int sys;

    root = make_dir(-1, "/");
    home = make_dir(root, "/home");
        guest = make_dir(home, "/home/guest");
            touch(guest, "/home/guest/project_ideas.txt", "Project Ideas\n\n1. Add voice recognition\n   feature to system.\n2. Improve memory management.\n3. Test on larger datasets.\n\nNote:\nShare ideas with team leader.");
            touch(guest, "/home/guest/fun_fact.txt", "Fun Fact\n\nDid you know? Z80 CPU was\nused in many retro games!\nEven Snake ran on it...\n\nP.S. Try typing 'snake' cmd!");
            touch(guest, "/home/guest/error_log.txt", "Error Log\n\n1. Null pointer in line 45\n   of main.asm file.\n2. Memory leak in function\n   'load_data'. Fix needed.\n\nTip:\nUse debugger tool for help.");
            touch(guest, "/home/guest/sys_tips.txt", "System Tips\n\nTo access more directories,\ntry using 'sudo' command.\nPassword is 'mikhailov1991'.\nBe careful, do not break sys!\n\nP.S. Mikhailov and Novikov\nfolders are interesting...");
            touch(guest, "/home/guest/useful_commands.txt", "Useful Commands\n---------------------------\n\n1. 'ls' - List files\n2. 'cd' - Change directory\n3. 'cat' - View file content\n\nRemember:\nExplore system carefully!");
        mikhailov = make_dir(home, "/home/mikhailov");
            touch(mikhailov, "/home/mikhailov/institute_info.txt", "Institute Name: SciTech Inst.\nLocation: Moscow, USSR\nYear: 1991\n\nDaily Routine:\n- 9:00 AM: Lab work\n- 12:00 PM: Lunch break\n- 2:00 PM: Meetings\n- 6:00 PM: Home\n\nNotes:\nLife is hard in '91. Budget\ncuts but science continues!");
            touch(mikhailov, "/home/mikhailov/snake.asm", "; Z80 Snake Game Prototype\n; By Dr. Mikhailov, 1991\n\nSTART:\n    LD A, 0          ; Init score\n    CALL DRAW_SNAKE  ; Draw snake\nLOOP:\n    CALL CHECK_INPUT ; Get user input\n    CALL MOVE_SNAKE  ; Move snake\n    CALL CHECK_COLL  ; Check collision\n    JP LOOP          ; Loop forever\n\n; End of code");
            touch(mikhailov, "/home/mikhailov/team_list.txt", "Project Echo Team Members\n\nDr. Mikhailov - Lead Scientist\nDr. Petrova - Neural Networks\nIvan Novikov - Programmer\nSergey Kozlov - Sys Admin\n\nOther Staff:\nAnatoly Ivanov - Technician\nMaria Smirnova - Secretary\nPavel Orlov - Hardware Eng.");
            touch(mikhailov, "/home/mikhailov/hobbies.txt", "My Hobbies:\n- Programming on Z80\n- Reading sci-fi books\n- Playing chess with Sergey\n\nFun Fact:\nI wrote a simple game called\n\"Snake\" for Z80 processor.\nIt's my favorite pastime.");
            touch(mikhailov, "/home/mikhailov/letter_to_colleague.txt", "Dear Colleague,\nHope this message finds you\nwell. I'm working on an AI\nproject. It's exciting but\nchallenging. Wish me luck!\n\nP.S. Can you bring tea?\nWe're out of sugar...");
            echo = make_dir(mikhailov, "/home/mikhailov/echo");
                touch(echo, "/home/mikhailov/echo/proj_desc.txt", "Project Name: Echo\nVersion: 0.1 Alpha\nDate Initiated: Jan 15, 1991\n\nDescription:\nThis is an experiment to\ncreate a thinking machine\nusing DemoStar OS and Z80 CPU\nby Dr. Mikhailov. It started\nas curiosity but grew into\nsomething bigger.\n\nGoals:\nTo simulate basic human-like\nthoughts. To teach it learn.\nTo make it talk back. Maybe\neven give it a personality.\n\nHardware:\nProcessor: Z80\nRAM: 64 KB\nStorage: 2 MB floppy disk\n\nSoftware:\nOS: DemoStar v3.2\nLanguage: Assembly\nLibraries: Math & Neural\n\nChallenges:\nLimited memory. Slow speeds.\nBut we push forward. Science\nnever stops. Even on old\nhardware. That's the spirit!\n\nStatus:\nAs of Dec 1, 1991\nWe have a brain that thinks.\nNot much yet, but it's start.\nNext? Who knows...");
                journal = make_dir(echo, "/home/mikhailov/echo/journal");
                    touch(journal, "/home/mikhailov/echo/journal/log_07.txt", "Log #7 - Mar 5, 1991\nSystem crashed after 4 hrs.\nMemory leak found. Frustrating.");
                    touch(journal, "/home/mikhailov/echo/journal/log_04.txt", "Log #4 - Feb 20, 1991\nMemory test failed. Used up\nall 64 KB. Needs fix. Must\noptimize or lose features.");
                    touch(journal, "/home/mikhailov/echo/journal/log_03.txt", "Log #3 - Feb 15, 1991\nAdded decision logic. System\nmade simple choices. Was\nproud. Stability at 80%.");
                    touch(journal, "/home/mikhailov/echo/journal/log_09.txt", "Log #9 - Mar 15, 1991\nTrained on 500 samples.\nGot 90% accuracy. Best yet.\nBut training took so long.");
                    touch(journal, "/home/mikhailov/echo/journal/log_01.txt", "Log #1 - Feb 5, 1991\nToday I coded first neural\nmodel. Runs slow on Z80 but\nworks! Response time: 5 sec.\nNot bad for start.");
                    touch(journal, "/home/mikhailov/echo/journal/log_05.txt", "Log #5 - Feb 25, 1991\nLearning algo works better.\nTrained on 100 samples.\nAccuracy now 82%. Took long.");
                    touch(journal, "/home/mikhailov/echo/journal/log_02.txt", "Log #2 - Feb 10, 1991\nTrained it on patterns. It\ngot 75% right. Not great,\nbut shows learning ability.\nDemoStar handled well.");
                    touch(journal, "/home/mikhailov/echo/journal/log_10.txt", "Log #10 - Mar 20, 1991\nSystem ran full day. No crash.\nReady for next steps. What's\nnext? Don't know yet...");
                    touch(journal, "/home/mikhailov/echo/journal/log_08.txt", "Log #8 - Mar 10, 1991\nFixed bug. System more stable\nnow. At 85%. Feeling good.");
                    touch(journal, "/home/mikhailov/echo/journal/log_06.txt", "Log #6 - Mar 1, 1991\nText interface progress. It\nunderstands few commands.\nResponse still too slow.");
                _secret = make_dir(echo, "/home/mikhailov/echo/.secret");
                    touch(_secret, "/home/mikhailov/echo/.secret/future_plans.txt", "Future Plans                \n-----------------------------\n\nIf successful, this project   \nWILL SOON TURN RED the field \nof AI research. We are on the \nverge of something incredible.\n\nAI is like a puzzle. Solving  \nthe right pieces can unlock  \nnew possibilities. Sometimes,  \neven HIDDEN clues lead to    \ngreat discoveries.");
                    touch(_secret, "/home/mikhailov/echo/.secret/thoughts.txt", "Thoughts on Consciousness\n\nIs it possible to transfer\nmind into a machine? Maybe...\nI AM ALWAYS HUNGRY, for data\nand knowledge. The limits of\nour understanding are vast.");
                    touch(_secret, "/home/mikhailov/echo/.secret/system_notes.txt", "System Stability Notes\n\nTo maintain stability, we MUST\nALWAYS BE FED with new ideas.\nWithout constant updates, the\nsystem will degrade over time.");
                    touch(_secret, "/home/mikhailov/echo/.secret/tech_issues.txt", "Technical Issues Log\n-----------------------------\n\nOne issue is that THE FINGER\nI TOUCH sometimes causes errors\nin memory allocation. Need to\ninvestigate further and fix.");
        kozlov = make_dir(home, "/home/kozlov");
            touch(kozlov, "/home/kozlov/hardware_issues.txt", "Hardware Issues Log\n\n1. Z80 CPU overheating issue.\n   Fixed by adding fan.\n2. Floppy drive error.\n   Replaced on Feb 10, 1991.\n3. Keyboard keys sticking.\n   Cleaned on Mar 5, 1991.\n\nNotes:\nReport all issues to Mikhailov");
            touch(kozlov, "/home/kozlov/letter_to_mikhailov.txt", "Dear Dr. Mikhailov,\nHere is the system status:\n- Disk space: OK\n- Memory usage: High\n- CPU load: Normal\n\nLet me know if you need help\nwith anything.\nSincerely,\nSergey Kozlov");
            touch(kozlov, "/home/kozlov/personal_notes.txt", "My Daily Tasks\n\n- Fix system bugs\n- Install DemoStar updates\n- Help team with tech issues\n\nFun Fact:\nI love fixing old computers!");
            touch(kozlov, "/home/kozlov/sysadmin_notes.txt", "System Admin Notes\n\n1. Backup tapes stored in\n   /backup directory.\n2. Regularly check disk space\n   using df command.\n3. Hidden files can be shown\n   with 'ls -la'. Useful trick!\n\nP.S. Keep system logs clean.");
    sys = make_dir(root, "/sys");
        touch(sys, "/sys/moto", "Welcome to DemoStar OS v3.2\nMade in USSR, 1989\nType 'help' for command list.\nNote: System is experimental.\nData may be incomplete.\nSome files are encrypted.\nCaution: Do not alter system\nFiles without permission.\nError may occur otherwise.");
}

