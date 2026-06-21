# Web-based-Calculator(HTML + CSS + C with CGI Runs on Apache Server)

This is a simple grid-style web calculator built with:
  HTML & CSS → Frontend interface
  C (CGI program) → Backend expression evaluator
  Apache HTTP Server → Runs the CGI program

** Features**

Grid-style calculator UI (like a real calculator)

Keyboard input support (0–9, + - * / ( ) .)

Enter = Calculate, Backspace = Delete, Esc = Clear

Error handling (division by zero, invalid syntax, missing parentheses)

Shows results cleanly (58 instead of 58.000000)

 **Project Structure**
project-folder/
│── index.html       # Calculator frontend
│── style.css        # Styling for calculator
│── calc.c           # C source code (CGI backend)
│── calc.cgi         # Compiled CGI binary
│── README.md        # Project documentation

 **Setup Instructions**
1. Install Apache with CGI enabled
   sudo apt update
   sudo apt install apache2 gcc -y
   sudo a2enmod cgi
   sudo systemctl restart apache2

2.Compile the C backend
   gcc calc.c -o calc.cgi
   sudo mv calc.cgi /usr/lib/cgi-bin/
   sudo chmod +x /usr/lib/cgi-bin/calc.cgi

3. Deploy frontend files
   sudo cp index.html /var/www/html/
   sudo cp style.css /var/www/html/

4. Open in browser
   http://localhost/index.html

** Tech Stack**

Frontend: HTML5, CSS3, JavaScript

Backend: C (CGI)

Server: Apache2

** Error Handling**

7*/2 → Error!

5/0 →  Error! (division by zero)

(2+3*4 →  Error! (missing bracket)
