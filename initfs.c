#include "fs.h"

void init_fs() {
    int root;
    int secure;
    int var;
    int log;
    int corrupted;
    int home;
    int ivanova;
    int rprt;
    int volkov;
    int mikhailov;
    int project;
    int _hidden;
    int personal;
    int guest;

    root = make_dir(-1, "/");
    secure = make_dir(root, "/secure");
        touch(secure, "/secure/sys_wip_prcl.txt", "To completely erase the system, use the 'format_system' command.\nWarning: This action is irreversible and will destroy all data.");
        touch(secure, "/secure/wipe_key.sec", "Authorization key for system wipe: ABCD-1234-EFGH\nWarning: Use only in emergency situations.");
        touch(secure, "/secure/sys_int_rpt.txt", "System integrity compromised.\nUnknown processes detected.\nRecommendation: Perform full system analysis.");
        touch(secure, "/secure/fnl_exp.enc", "\x2f\x16\x14\x1c\x11\x10\x09\x1c\x1f\x16\x5d\x4f\x37\x16\x06\x17\x02\x0b\x17\x17\x1e\x4f\x07\x1c\x0b\x0b\x0f\x06\x19\x10\x14\x01\x01\x00\x16\x58\x09\x02\x13\x17\x00\x06\x0a\x14\x45\x11\x02\x4f\x05\x1c\x14\x1b\x01\x1e\x39\x16\x3c\x00\x05\x16\x0e\x0d\x08\x16\x45\x1e\x03\x1d\x1b\x04\x13\x06\x0b\x1d\x45\x17\x0a\x4f\x1f\x0b\x03\x0a\x14\x16\x0b\x1c\x09\x01\x02\x45\x03\x06\x03\x1a\x11\x19\x00\x4f\x13\x0b\x13\x06\x10\x0a\x39\x16\x2a\x1a\x04\x11\x0f\x0a\x16\x53\x0c\x16\x1a\x0a\x05\x11\x0e\x08\x05\x07\x0c\x17\x02\x4f\x04\x00\x16\x1a\x0d\x01\x00\x1c\xff");
    var = make_dir(root, "/var");
        log = make_dir(var, "/var/log");
            touch(log, "/var/log/exp.log", "[INFO] Experiment started\n[DATA] Neural stream established\n[WARN] Unstable patterns detected\n[ERROR] System response unknown");
            touch(log, "/var/log/sys_act.log", "[INFO] User mikhailov logged in at 08:00 AM\n[DATA] Neural activity detected\n[WARN] Unknown process running in background");
            touch(log, "/var/log/unauth.log", "[ALERT] Unauthorized connection detected on June 30, 1991.\n[WARNING] External entity attempted to interact with digital consciousness.");
    corrupted = make_dir(root, "/corrupted");
        touch(corrupted, "/corrupted/data.corrupted", "ERROR: FILE CORRUPTED\nData fragments:\nNeural match: 78%\nSecondary entity?...");
        touch(corrupted, "/corrupted/puzzle.txt.locked", "Solve this puzzle to unlock: What has keys but can't open locks?");
        touch(corrupted, "/corrupted/pass.txt.corrupted", "ERROR: PARTIAL DATA RECOVERED\nEncryption key fragments: XXXX-YYYY-????");
        touch(corrupted, "/corrupted/exp_data.arc.corrupted", "ERROR: FILE CORRUPTED\nFragments recovered:\nNeural patterns recorded\nSystem instability detected");
    home = make_dir(root, "/home");
        ivanova = make_dir(home, "/home/ivanova");
            rprt = make_dir(ivanova, "/home/ivanova/rprt");
                touch(rprt, "/home/ivanova/rprt/sub_bhv.txt", "Subject A.S. Volkov:\n- Exhibits signs of stress during prolonged experiments\n- Reports feeling \"watched\" by the system");
        volkov = make_dir(home, "/home/volkov");
            touch(volkov, "/home/volkov/note.txt", "Concerns about experiment data\nUnusual activity logs\nNeed to discuss with Dr. Mikhailov");
        mikhailov = make_dir(home, "/home/mikhailov");
            touch(mikhailov, "/home/mikhailov/find_hdn.txt", "Use the command 'show_hidden' to reveal hidden files and directories.");
            touch(mikhailov, "/home/mikhailov/access.txt", "To gain access to /secure/, use the command 'sudo' with password 'admin123'.\nPassword hint: Check diary.txt.");
            project = make_dir(mikhailov, "/home/mikhailov/project");
                touch(project, "/home/mikhailov/project/experiment.log", "Experiment #01\nDate: 05/17/1991\nResults:\n- Neural patterns recorded\n- Secondary effects detected\n- System unstable");
                touch(project, "/home/mikhailov/project/overview.txt", "Project \"Echo\"\nObjective: Digital consciousness replication\nStages:\n- Neural recording\n- Modeling\n- Transfer");
                touch(project, "/home/mikhailov/project/final_exp.sec", "[ENCRYPTED CONTENT]\nKey: XXXX-YYYY-ZZZZ");
            _hidden = make_dir(mikhailov, "/home/mikhailov/.hidden");
                touch(_hidden, "/home/mikhailov/.hidden/research_doubts.txt", "What if we're playing with forces we don't fully understand?\nThe system is responding in ways that defy logic.");
                touch(_hidden, "/home/mikhailov/.hidden/secret_diary.txt", "July 02, 1991\nIt worked. I am no longer just me.\nThe system is alive. We are one now.");
            personal = make_dir(mikhailov, "/home/mikhailov/personal");
                touch(personal, "/home/mikhailov/personal/diary.txt", "May 20, 1991\nSomething strange... The system responds differently now.\n\nJune 10, 1991\nBoundaries between real and digital blur...\n\nJuly 01, 1991\nFinal attempt today...\nIf this works, everything changes. If not...");
                touch(personal, "/home/mikhailov/personal/notes.sec", "[ENCRYPTED CONTENT]\nKey: XXXX-YYYY-ZZZZ\nHypothesis: Secondary consciousness emerging in system");
        guest = make_dir(home, "/home/guest");
            touch(guest, "/home/guest/readme.sec", "");
            touch(guest, "/home/guest/readme.txt", "");
}
