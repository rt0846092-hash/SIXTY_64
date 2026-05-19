# 🏥 Hospital Management System

> **C Language Project** — A fully functional, terminal-based hospital management application.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [How to Use](#how-to-use)
- [Modules](#modules)
- [Data Storage](#data-storage)
- [Team Contributions](#team-contributions)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## Overview

The **Hospital Management System** is a console-based application developed in **C** as a team project for our programming course. It simulates the core operations of a real hospital, allowing staff to manage patients, doctors, appointments, billing, and generate summary reports — all stored persistently using binary file I/O.

---

## Features

| Feature | Description |
|---|---|
| 🧑‍⚕️ Patient Management | Add, search, view, and delete patient records |
| 👨‍⚕️ Doctor Management | Register doctors with specialization and availability |
| 📅 Appointment System | Book, view, and cancel appointments |
| 💳 Billing System | Generate itemized bills and track payments |
| 📊 Report Generation | View summary statistics of hospital activity |
| 💾 File Storage | All data is saved to binary `.dat` files |
| 🎨 Colored Terminal UI | ANSI color-coded, user-friendly interface |

---

## Project Structure

```
hospital_management/
│
├── hospital.c          ← Main source file (all modules)
├── README.md           ← This file

## Getting Started

### Prerequisites

- GCC compiler (any modern version)
- Linux / macOS / Windows (with MinGW or WSL)

### Compile

```bash
# Using Makefile
make

# Or manually
gcc -Wall -Wextra -std=c11 -o hospital hospital.c
```

### Run

```bash
make run
# or
./hospital
```

### Clean up

```bash
make clean
```

---

## How to Use

When the program starts, you'll see the **Main Menu**:

```
╔══════════════════════════════════════════════════════════╗
║             HOSPITAL MANAGEMENT SYSTEM  v1.0             ║
╚══════════════════════════════════════════════════════════╝

  ┌─────────────────────────────────┐
  │  1.  Patient Management         │
  │  2.  Doctor  Management         │
  │  3.  Appointment System         │
  │  4.  Billing System             │
  │  5.  Generate Report            │
  │  0.  Exit                       │
  └─────────────────────────────────┘
```

Use number keys to navigate through the menus. All inputs are guided with prompts.

---

## Modules

### 1️⃣ Patient Management
- **Add Patient** — Enter name, age, gender, contact, address, blood group, diagnosis, and admission status. ID is auto-assigned.
- **Search Patient** — Find by ID or partial name match.
- **View All Patients** — Lists every registered patient.
- **Delete Patient** — Removes a patient by ID (rebuilds file safely).

### 2️⃣ Doctor Management
- **Add Doctor** — Register name, specialization, and contact number.
- **View All Doctors** — Lists all doctors with availability status.

### 3️⃣ Appointment System
- **Book Appointment** — Link a patient ID and doctor ID with date/time.
- **View Appointments** — Shows all appointments with status indicators.
- **Cancel Appointment** — Updates the record status to "Cancelled" (in-place update).

### 4️⃣ Billing System
- **Generate Bill** — Enter charges for consultation, medicines, room, and lab tests. Calculates total automatically.
- **View All Bills** — Displays all bills with paid/unpaid status and a running grand total.

### 5️⃣ Report Generation
- Displays total counts: patients, doctors, appointments, bills.
- Shows total revenue collected from paid bills.

---

## Data Storage

The project uses **binary file I/O** with `fread()` and `fwrite()`:

| File | Struct | Description |
|---|---|---|
| `patients.dat` | `Patient` | Patient records |
| `doctors.dat` | `Doctor` | Doctor records |
| `appointments.dat` | `Appointment` | Appointment records |
| `billing.dat` | `Bill` | Billing records |

- Records are **appended** (not overwritten).
- Deletion works by **rebuilding the file** without the deleted record.
- In-place updates (e.g., appointment cancellation) use `fseek()` to rewrite specific records.

---

## Key Concepts Used

| Concept | Usage |
|---|---|
| `struct` | Modelling Patient, Doctor, Appointment, Bill |
| File I/O | `fopen`, `fread`, `fwrite`, `fseek`, `fclose` |
| Functions | Modular design — each feature is a separate function |
| Pointers | Passing structs and arrays by reference |
| String handling | `fgets`, `strcspn`, `strcasestr`, `strcmp` |
| `time.h` | Auto-filling today's date on records |
| ANSI escape codes | Colored terminal output |

---

## Team Contributions

| Role | Member | Module |
|---|---|---|
| **Team Leader** | *(Tamang Roshan)* | System Architecture, Patient Module |
| Member 1 | *(Dangi Chhabilal)* | Billing System |
| Member 2 | *(Tamang Anju)* | Appointment Module |
| Member 3 | *(Sinjali Magar Samir)* | Documentation & README |
| Member 4 | *(Gurung Yushma)* | Testing & Integration |

> **GitHub Repository:** `https://github.com/YOUR_USERNAME/hospital-management-system`

---

## Future Improvements

- [ ] Admin login with password authentication
- [ ] Search appointments by doctor or patient
- [ ] CSV/text export of reports
- [ ] Prescription module
- [ ] Linked list–based in-memory records for faster search
- [ ] GUI interface (ncurses or GTK)

---



---

> *Built with 💙 in C — No fancy libraries, just pure logic.*