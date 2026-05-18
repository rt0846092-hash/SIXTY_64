
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Colour helpers (ANSI) */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[1;36m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"

/* ── File names ── */
#define PATIENT_FILE     "patients.dat"
#define DOCTOR_FILE      "doctors.dat"
#define APPOINTMENT_FILE "appointments.dat"
#define BILLING_FILE     "billing.dat"

/* ── Constants ── */
#define MAX_NAME   60
#define MAX_ADDR   120
#define MAX_DIAG   200
#define MAX_SPEC   60
#define MAX_DATE   20

/* ── Structures ── */
typedef struct {
    int    id;
    char   name[MAX_NAME];
    int    age;
    char   gender;          /* 'M' / 'F' / 'O' */
    char   contact[16];
    char   address[MAX_ADDR];
    char   blood_group[5];
    char   diagnosis[MAX_DIAG];
    char   admit_date[MAX_DATE];
    int    is_admitted;     /* 1 = admitted, 0 = out-patient */
} Patient;

typedef struct {
    int    id;
    char   name[MAX_NAME];
    char   specialization[MAX_SPEC];
    char   contact[16];
    int    available;       /* 1 = available */
} Doctor;

typedef struct {
    int    id;
    int    patient_id;
    int    doctor_id;
    char   date[MAX_DATE];
    char   time_slot[12];
    char   status[16];      /* "Scheduled" / "Completed" / "Cancelled" */
} Appointment;

typedef struct {
    int    id;
    int    patient_id;
    char   date[MAX_DATE];
    float  consultation_fee;
    float  medicine_charge;
    float  room_charge;
    float  test_charge;
    float  total;
    int    paid;            /* 1 = paid */
} Bill;

/* ══════════════════════════════════════════
   UTILITY HELPERS
══════════════════════════════════════════ */

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void press_enter(void) {
    printf("\n" YELLOW "  Press Enter to continue..." RESET);
    getchar();
}

void print_header(const char *title) {
    clear_screen();
    int len = (int)strlen(title);
    int pad = (60 - len) / 2;
    printf(CYAN);
    printf("  ╔══════════════════════════════════════════════════════════╗\n");
    printf("  ║%*s%s%*s║\n", pad, "", title, 60 - pad - len, "");
    printf("  ╚══════════════════════════════════════════════════════════╝\n");
    printf(RESET);
}

void print_divider(void) {
    printf(CYAN "  ──────────────────────────────────────────────────────────\n" RESET);
}

void get_today(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, MAX_DATE, "%Y-%m-%d", tm_info);
}

/*
 * POST-ACTION PROMPT (standard — 2 options)
 * Returns 1 → stay in current sub-menu.
 * Returns 0 → return to Main Menu.
 */
 int post_action_prompt(void) {
    printf("\n");
    printf(CYAN "  ┌──────────────────────────────────┐\n" RESET);
    printf(CYAN "  │" RESET "  " BOLD "1." RESET "  Stay in this menu           " CYAN "│\n" RESET);
    printf(CYAN "  │" RESET "  " BOLD "0." RESET "  Return to Main Menu         " CYAN "│\n" RESET);
    printf(CYAN "  └──────────────────────────────────┘\n" RESET);
    printf("  Choice: ");
    int ch;
    scanf("%d", &ch);
    while (getchar() != '\n');
    return (ch == 1);
}

/*
 * POST-REMOVE PROMPT (3 options — used after Remove Patient)
 * Returns:
 *   2 → exit the program completely
 *   0 → return to Main Menu
 *   1 → back to the same (Patient) menu  [default]
 */
 int post_remove_prompt(void) {
    printf("\n");
    printf(CYAN "  ┌──────────────────────────────────────┐\n" RESET);
    printf(CYAN "  │" RESET "  " BOLD "1." RESET "  Exit completely               " CYAN "│\n" RESET);
    printf(CYAN "  │" RESET "  " BOLD "2." RESET "  Return to Main Menu           " CYAN "│\n" RESET);
    printf(CYAN "  │" RESET "  " BOLD "3." RESET "  Back to Patient Menu          " CYAN "│\n" RESET);
    printf(CYAN "  └──────────────────────────────────────┘\n" RESET);
    printf("  Choice: ");
    int ch;
    scanf("%d", &ch);
    while (getchar() != '\n');
    return ch;   /* caller checks: 1=exit, 2=main menu, 3=stay */
}

/* ══════════════════════════════════════════
   PATIENT FUNCTIONS
══════════════════════════════════════════ */

 int count_patients(void) {
    FILE *f = fopen(PATIENT_FILE, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    int n = (int)(ftell(f) / sizeof(Patient));
    fclose(f);
    return n;
}

void add_patient(void) {
    print_header("ADD NEW PATIENT");

    Patient p;
    memset(&p, 0, sizeof(p));
    p.id = count_patients() + 1;

    printf(GREEN  "  ┌──────────────────────────────────┐\n" RESET);
    printf(GREEN  "  │  Auto-assigned Patient ID : " BOLD "%-4d" RESET GREEN "  │\n" RESET, p.id);
    printf(GREEN  "  │  (Note this ID for future use)   │\n" RESET);
    printf(GREEN  "  └──────────────────────────────────┘\n\n" RESET);

    printf("  Full Name   : "); fgets(p.name,       MAX_NAME,  stdin); p.name[strcspn(p.name, "\n")]             = 0;
    printf("  Age         : "); scanf("%d", &p.age); while (getchar() != '\n');
    printf("  Gender (M/F/O): "); p.gender = (char)toupper(getchar()); while (getchar() != '\n');
    printf("  Contact No  : "); fgets(p.contact,    16,        stdin); p.contact[strcspn(p.contact, "\n")]       = 0;
    printf("  Address     : "); fgets(p.address,    MAX_ADDR,  stdin); p.address[strcspn(p.address, "\n")]       = 0;
    printf("  Blood Group : "); fgets(p.blood_group, 5,        stdin); p.blood_group[strcspn(p.blood_group,"\n")]= 0;
    printf("  Diagnosis   : "); fgets(p.diagnosis,  MAX_DIAG,  stdin); p.diagnosis[strcspn(p.diagnosis, "\n")]   = 0;
    printf("  Admitted? (1=Yes / 0=No): "); scanf("%d", &p.is_admitted); while (getchar() != '\n');

    get_today(p.admit_date);

    FILE *f = fopen(PATIENT_FILE, "ab");
    if (!f) { printf(RED "  Error opening file!\n" RESET); return; }
    fwrite(&p, sizeof(p), 1, f);
    fclose(f);

    printf(GREEN "\n  ✓ Patient added successfully!\n" RESET);
    printf(GREEN "    Name: %s  |  ID: %d\n" RESET, p.name, p.id);
}

 void display_patient(const Patient *p) {
    print_divider();
    printf(BOLD "  Patient #%d\n" RESET, p->id);
    printf("  Name        : %s\n",  p->name);
    printf("  Age         : %d  | Gender: %c  | Blood: %s\n", p->age, p->gender, p->blood_group);
    printf("  Contact     : %s\n",  p->contact);
    printf("  Address     : %s\n",  p->address);
    printf("  Diagnosis   : %s\n",  p->diagnosis);
    printf("  Admitted    : %s  | Date: %s\n", p->is_admitted ? "Yes" : "No", p->admit_date);
}

void search_patient(void) {
    print_header("SEARCH PATIENT");

    printf("  Search by:\n");
    printf("   1. Patient ID\n");
    printf("   2. Name\n");
    printf("  Choice: ");
    int ch; scanf("%d", &ch); while (getchar() != '\n');

    FILE *f = fopen(PATIENT_FILE, "rb");
    if (!f) { printf(RED "  No patient records found.\n" RESET); press_enter(); return; }

    Patient p;
    int found = 0;

    if (ch == 1) {
        int id; printf("  Enter Patient ID: "); scanf("%d", &id); while (getchar() != '\n');
        while (fread(&p, sizeof(p), 1, f) == 1) {
            if (p.id == id) { display_patient(&p); found = 1; }
        }
    } else {
        char name[MAX_NAME]; printf("  Enter Name (partial ok): "); fgets(name, MAX_NAME, stdin); name[strcspn(name, "\n")] = 0;
        while (fread(&p, sizeof(p), 1, f) == 1) {
            if (strstr(p.name, name)) { display_patient(&p); found = 1; }
        }
    }
    fclose(f);

    if (!found) printf(RED "\n  No matching patient found.\n" RESET);
}

void view_all_patients(void) {
    print_header("ALL PATIENTS");

    FILE *f = fopen(PATIENT_FILE, "rb");
    if (!f) { printf(RED "  No records found.\n" RESET); press_enter(); return; }

    Patient p;
    int cnt = 0;
    while (fread(&p, sizeof(p), 1, f) == 1) { display_patient(&p); cnt++; }
    fclose(f);

    if (cnt == 0) printf(RED "  No patient records.\n" RESET);
    else printf(GREEN "\n  Total Patients: %d\n" RESET, cnt);
}

/*
 * REMOVE PATIENT  (renamed from delete_patient)
 * -----------------------------------------------
 * After the operation, post_remove_prompt() offers
 * three choices:
 *   1 → exit the whole program
 *   2 → return to Main Menu
 *   3 → stay in the Patient sub-menu
 *
 * Returns the raw choice so patient_menu() can act on it.
 */
int remove_patient(void) {
    print_header("REMOVE PATIENT RECORD");

    int id; printf("  Enter Patient ID to remove record: "); scanf("%d", &id); while (getchar() != '\n');

    FILE *f  = fopen(PATIENT_FILE, "rb");
    FILE *tf = fopen("tmp_patients.dat", "wb");
    if (!f || !tf) { printf(RED "  File error.\n" RESET); press_enter(); return 3; }

    Patient p; int found = 0;
    while (fread(&p, sizeof(p), 1, f) == 1) {
        if (p.id == id) { found = 1; printf(YELLOW "  Removed Patient Record: %s\n" RESET, p.name); }
        else fwrite(&p, sizeof(p), 1, tf);
    }
    fclose(f); fclose(tf);
    remove(PATIENT_FILE);
    rename("tmp_patients.dat", PATIENT_FILE);

    if (!found) printf(RED "  Patient ID not found.\n" RESET);

    return post_remove_prompt();   /* 1=exit, 2=main, 3=stay */
}

/* ══════════════════════════════════════════
   DOCTOR FUNCTIONS
══════════════════════════════════════════ */

int count_doctors(void) {
    FILE *f = fopen(DOCTOR_FILE, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    int n = (int)(ftell(f) / sizeof(Doctor));
    fclose(f);
    return n;
}

void add_doctor(void) {
    print_header("ADD NEW DOCTOR");
    Doctor d; memset(&d, 0, sizeof(d));
    d.id = count_doctors() + 1;

    printf(GREEN "  ┌──────────────────────────────────┐\n" RESET);
    printf(GREEN "  │  Auto-assigned Doctor ID  : " BOLD "%-4d" RESET GREEN "  │\n" RESET, d.id);
    printf(GREEN "  └──────────────────────────────────┘\n\n" RESET);

    printf("  Full Name       : "); fgets(d.name,           MAX_NAME, stdin); d.name[strcspn(d.name, "\n")]                         = 0;
    printf("  Specialization  : "); fgets(d.specialization, MAX_SPEC, stdin); d.specialization[strcspn(d.specialization, "\n")]     = 0;
    printf("  Contact No      : "); fgets(d.contact,        16,       stdin); d.contact[strcspn(d.contact, "\n")]                   = 0;
    d.available = 1;

    FILE *f = fopen(DOCTOR_FILE, "ab");
    if (!f) { printf(RED "  Error!\n" RESET); return; }
    fwrite(&d, sizeof(d), 1, f);
    fclose(f);

    printf(GREEN "\n  ✓ Doctor added! Name: %s  |  ID: %d\n" RESET, d.name, d.id);
}

void view_all_doctors(void) {
    print_header("ALL DOCTORS");
    FILE *f = fopen(DOCTOR_FILE, "rb");
    if (!f) { printf(RED "  No doctor records.\n" RESET); press_enter(); return; }

    Doctor d; int cnt = 0;
    while (fread(&d, sizeof(d), 1, f) == 1) {
        print_divider();
        printf(BOLD "  Dr. %s  [ID: %d]\n" RESET, d.name, d.id);
        printf("  Specialization : %s\n",  d.specialization);
        printf("  Contact        : %s\n",  d.contact);
        printf("  Status         : %s\n",  d.available ? GREEN "Available" RESET : RED "Unavailable" RESET);
        cnt++;
    }
    fclose(f);
    if (cnt == 0) printf(RED "  No doctors found.\n" RESET);
}

/* ══════════════════════════════════════════
   APPOINTMENT FUNCTIONS
══════════════════════════════════════════ */

int count_appointments(void) {
    FILE *f = fopen(APPOINTMENT_FILE, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    int n = (int)(ftell(f) / sizeof(Appointment));
    fclose(f);
    return n;
}

void book_appointment(void) {
    print_header("BOOK APPOINTMENT");

    Appointment a; memset(&a, 0, sizeof(a));
    a.id = count_appointments() + 1;

    printf("  Patient ID  : "); scanf("%d", &a.patient_id);  while (getchar() != '\n');
    printf("  Doctor  ID  : "); scanf("%d", &a.doctor_id);   while (getchar() != '\n');
    printf("  Date (YYYY-MM-DD) : "); fgets(a.date, MAX_DATE, stdin); a.date[strcspn(a.date, "\n")] = 0;
    printf("  Time Slot (e.g. 10:00 AM): "); fgets(a.time_slot, 12, stdin); a.time_slot[strcspn(a.time_slot, "\n")] = 0;
    strcpy(a.status, "Scheduled");

    FILE *f = fopen(APPOINTMENT_FILE, "ab");
    if (!f) { printf(RED "  Error!\n" RESET); return; }
    fwrite(&a, sizeof(a), 1, f);
    fclose(f);

    printf(GREEN "\n  ✓ Appointment booked! ID = %d\n" RESET, a.id);
}

void view_appointments(void) {
    print_header("ALL APPOINTMENTS");
    FILE *f = fopen(APPOINTMENT_FILE, "rb");
    if (!f) { printf(RED "  No appointments.\n" RESET); press_enter(); return; }

    Appointment a; int cnt = 0;
    while (fread(&a, sizeof(a), 1, f) == 1) {
        print_divider();
        printf("  Appt #%d  |  Patient: %d  |  Doctor: %d\n", a.id, a.patient_id, a.doctor_id);
        printf("  Date: %s  |  Time: %s  |  Status: ", a.date, a.time_slot);
        if (strcmp(a.status, "Scheduled") == 0)       printf(YELLOW "%s\n" RESET, a.status);
        else if (strcmp(a.status, "Completed") == 0)  printf(GREEN  "%s\n" RESET, a.status);
        else                                           printf(RED    "%s\n" RESET, a.status);
        cnt++;
    }
    fclose(f);
    if (cnt == 0) printf(RED "  No records.\n" RESET);
}

 void cancel_appointment(void) {
    print_header("CANCEL APPOINTMENT");
    int id; printf("  Enter Appointment ID to cancel: "); scanf("%d", &id); while (getchar() != '\n');

    FILE *f = fopen(APPOINTMENT_FILE, "r+b");
    if (!f) { printf(RED "  No records.\n" RESET); press_enter(); return; }

    Appointment a; int found = 0;
    while (fread(&a, sizeof(a), 1, f) == 1) {
        if (a.id == id) {
            strcpy(a.status, "Cancelled");
            fseek(f, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, f);
            found = 1;
            printf(GREEN "  ✓ Appointment #%d cancelled.\n" RESET, id);
            break;
        }
    }
    fclose(f);
    if (!found) printf(RED "  Appointment not found.\n" RESET);
}

/* ══════════════════════════════════════════
   BILLING FUNCTIONS
══════════════════════════════════════════ */

 int count_bills(void) {
    FILE *f = fopen(BILLING_FILE, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    int n = (int)(ftell(f) / sizeof(Bill));
    fclose(f);
    return n;
}

void generate_bill(void) {
    print_header("GENERATE BILL");

    Bill b; memset(&b, 0, sizeof(b));
    b.id = count_bills() + 1;

    printf("  Patient ID          : "); scanf("%d", &b.patient_id); while (getchar() != '\n');

    /* ── Look up whether this patient is admitted ── */
    int is_admitted = 0;
    char pat_name[MAX_NAME] = "Unknown";
    {
        FILE *pf = fopen(PATIENT_FILE, "rb");
        if (pf) {
            Patient p;
            while (fread(&p, sizeof(p), 1, pf) == 1) {
                if (p.id == b.patient_id) {
                    is_admitted = p.is_admitted;
                    strncpy(pat_name, p.name, MAX_NAME - 1);
                    break;
                }
            }
            fclose(pf);
        }
    }

    /* Show patient admission status before entering charges */
    printf("\n");
    if (is_admitted)
        printf(GREEN  "  Patient \"%s\" is ADMITTED — room charges applicable.\n\n" RESET, pat_name);
    else
        printf(YELLOW "  Patient \"%s\" is OUT-PATIENT — room charges will be set to Rs. 0.00.\n\n" RESET, pat_name);

    printf("  Consultation Fee    : Rs. "); scanf("%f", &b.consultation_fee); while (getchar() != '\n');
    printf("  Medicine Charges    : Rs. "); scanf("%f", &b.medicine_charge);  while (getchar() != '\n');

    if (is_admitted) {
        printf("  Room Charges        : Rs. "); scanf("%f", &b.room_charge); while (getchar() != '\n');
    } else {
        b.room_charge = 0.0f;
        printf(YELLOW "  Room Charges        : Rs. 0.00  (skipped — out-patient)\n" RESET);
    }

    printf("  Lab/Test Charges    : Rs. "); scanf("%f", &b.test_charge); while (getchar() != '\n');

    b.total = b.consultation_fee + b.medicine_charge + b.room_charge + b.test_charge;
    get_today(b.date);

    printf(GREEN "\n  ─────────── BILL SUMMARY ───────────\n");
    printf("  Consultation : Rs. %.2f\n",  b.consultation_fee);
    printf("  Medicine     : Rs. %.2f\n",  b.medicine_charge);
    if (is_admitted)
        printf("  Room         : Rs. %.2f\n",  b.room_charge);
    else
        printf(YELLOW "  Room         : Rs. 0.00  (out-patient)\n" RESET GREEN);
    printf("  Tests        : Rs. %.2f\n",  b.test_charge);
    printf(BOLD "  TOTAL        : Rs. %.2f\n" RESET GREEN, b.total);
    printf("  ─────────────────────────────────────\n" RESET);

    printf("  Mark as paid? (1=Yes / 0=No): "); scanf("%d", &b.paid); while (getchar() != '\n');

    FILE *f = fopen(BILLING_FILE, "ab");
    if (!f) { printf(RED "  Error!\n" RESET); return; }
    fwrite(&b, sizeof(b), 1, f);
    fclose(f);

    printf(GREEN "  ✓ Bill saved. Bill ID = %d\n" RESET, b.id);
}

void view_bills(void) {
    print_header("ALL BILLS");
    FILE *f = fopen(BILLING_FILE, "rb");
    if (!f) { printf(RED "  No billing records.\n" RESET); press_enter(); return; }

    Bill b; float grand = 0;
    while (fread(&b, sizeof(b), 1, f) == 1) {
        print_divider();
        printf("  Bill #%d  |  Patient: %d  |  Date: %s\n", b.id, b.patient_id, b.date);
        printf("  Charges → Consult: %.2f  |  Medicine: %.2f  |  Room: %.2f  |  Tests: %.2f\n",
               b.consultation_fee, b.medicine_charge, b.room_charge, b.test_charge);
        printf(BOLD "  TOTAL: Rs. %.2f  |  Status: " RESET, b.total);
        printf("%s\n", b.paid ? GREEN "PAID" RESET : RED "UNPAID" RESET);
        grand += b.total;
    }
    fclose(f);
    print_divider();
    printf(BOLD GREEN "  Grand Total (all bills): Rs. %.2f\n" RESET, grand);
}

/* ══════════════════════════════════════════
   REPORT FUNCTIONS
══════════════════════════════════════════ */

void generate_report(void) {
    print_header("SYSTEM REPORT");

    int patients     = count_patients();
    int doctors      = count_doctors();
    int appointments = count_appointments();
    int bills        = count_bills();

    float total_revenue = 0;
    FILE *f = fopen(BILLING_FILE, "rb");
    if (f) { Bill b; while (fread(&b, sizeof(b), 1, f) == 1) if (b.paid) total_revenue += b.total; fclose(f); }

    printf(CYAN "\n  ╔══════════════════════════════════╗\n");
    printf("  ║      HOSPITAL SUMMARY REPORT     ║\n");
    printf("  ╚══════════════════════════════════╝\n\n" RESET);

    printf(BOLD "  Registered Patients    : " RESET GREEN "%d\n" RESET, patients);
    printf(BOLD "  Registered Doctors     : " RESET GREEN "%d\n" RESET, doctors);
    printf(BOLD "  Total Appointments     : " RESET YELLOW "%d\n" RESET, appointments);
    printf(BOLD "  Total Bills Generated  : " RESET YELLOW "%d\n" RESET, bills);
    printf(BOLD "  Total Revenue Collected: " RESET MAGENTA "Rs. %.2f\n" RESET, total_revenue);

    press_enter();
}

/* ══════════════════════════════════════════
   MENUS
══════════════════════════════════════════ */

/*
 * Forward-declare main so patient_menu can call exit()
 * indirectly through the return value of remove_patient().
 * No forward declaration needed — we just use exit(0).
 */

 void patient_menu(void) {
    int ch;
    do {
        print_header("PATIENT MANAGEMENT");
        printf("  " BOLD "1." RESET " Add Patient\n");
        printf("  " BOLD "2." RESET " Search Patient\n");
        printf("  " BOLD "3." RESET " View All Patients\n");
        printf("  " BOLD "4." RESET " Remove Patient Record\n");   /* renamed */
        printf("  " BOLD "0." RESET " Back to Main Menu\n\n");
        printf("  Choice: "); scanf("%d", &ch); while (getchar() != '\n');

        int stay = 1;
        switch (ch) {
            case 1: add_patient();       stay = post_action_prompt(); break;
            case 2: search_patient();    stay = post_action_prompt(); break;
            case 3: view_all_patients(); stay = post_action_prompt(); break;
            case 4: {
                int action = remove_patient();
                if      (action == 1) { printf(GREEN "\n  Goodbye!\n\n" RESET); exit(0); }
                else if (action == 2) { stay = 0; }   /* go to main menu */
                else                  { stay = 1; }   /* back to patient menu */
                break;
            }

            case 0: break;
            default: printf(RED "  Invalid choice!\n" RESET); press_enter();
        }
        if (!stay) ch = 0;
    } while (ch != 0);
}

void doctor_menu(void) {
    int ch;
    do {
        print_header("DOCTOR MANAGEMENT");
        printf("  " BOLD "1." RESET " Add Doctor\n");
        printf("  " BOLD "2." RESET " View All Doctors\n");
        printf("  " BOLD "0." RESET " Back to Main Menu\n\n");
        printf("  Choice: "); scanf("%d", &ch); while (getchar() != '\n');

        int stay = 1;
        switch (ch) {
            case 1: add_doctor();       stay = post_action_prompt(); break;
            case 2: view_all_doctors(); stay = post_action_prompt(); break;
            case 0: break;
            default: printf(RED "  Invalid choice!\n" RESET); press_enter();
        }
        if (!stay) ch = 0;
    } while (ch != 0);
}

void appointment_menu(void) {
    int ch;
    do {
        print_header("APPOINTMENT MANAGEMENT");
        printf("  " BOLD "1." RESET " Book Appointment\n");
        printf("  " BOLD "2." RESET " View Appointments\n");
        printf("  " BOLD "3." RESET " Cancel Appointment\n");
        printf("  " BOLD "0." RESET " Back to Main Menu\n\n");
        printf("  Choice: "); scanf("%d", &ch); while (getchar() != '\n');

        int stay = 1;
        switch (ch) {
            case 1: book_appointment();   stay = post_action_prompt(); break;
            case 2: view_appointments();  stay = post_action_prompt(); break;
            case 3: cancel_appointment(); stay = post_action_prompt(); break;
            case 0: break;
            default: printf(RED "  Invalid choice!\n" RESET); press_enter();
        }
        if (!stay) ch = 0;
    } while (ch != 0);
}

void billing_menu(void) {
    int ch;
    do {
        print_header("BILLING SYSTEM");
        printf("  " BOLD "1." RESET " Generate Bill\n");
        printf("  " BOLD "2." RESET " View All Bills\n");
        printf("  " BOLD "0." RESET " Back to Main Menu\n\n");
        printf("  Choice: "); scanf("%d", &ch); while (getchar() != '\n');

        int stay = 1;
        switch (ch) {
            case 1: generate_bill(); stay = post_action_prompt(); break;
            case 2: view_bills();    stay = post_action_prompt(); break;
            case 0: break;
            default: printf(RED "  Invalid choice!\n" RESET); press_enter();
        }
        if (!stay) ch = 0;
    } while (ch != 0);
}

/* ══════════════════════════════════════════
   MAIN
══════════════════════════════════════════ */

int main(void) {
    int ch;
    do {
        print_header("HOSPITAL MANAGEMENT SYSTEM");
        printf("\n");
        printf("  " CYAN "┌─────────────────────────────────┐\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "1." RESET "  Patient Management          " CYAN "│\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "2." RESET "  Doctor  Management          " CYAN "│\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "3." RESET "  Appointment System          " CYAN "│\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "4." RESET "  Billing System              " CYAN "│\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "5." RESET "  Generate Report             " CYAN "│\n" RESET);
        printf("  " CYAN "│" RESET "  " BOLD "0." RESET "  Exit                        " CYAN "│\n" RESET);
        printf("  " CYAN "└─────────────────────────────────┘\n\n" RESET);
        printf("  Your choice: ");
        scanf("%d", &ch); while (getchar() != '\n');

        switch (ch) {
            case 1: patient_menu();     break;
            case 2: doctor_menu();      break;
            case 3: appointment_menu(); break;
            case 4: billing_menu();     break;
            case 5: generate_report();  break;
            case 0: break;
            default: printf(RED "  Invalid choice!\n" RESET); press_enter();
        }
    } while (ch != 0);

    printf(GREEN "\n  Thank you for using Hospital Management System!\n\n" RESET);
    return 0;
}