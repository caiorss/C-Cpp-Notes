;; C++/ Emacs helper functions and commands. 
;;
;; Load this file with  'M-x load-file cppEmacs.el'
;;
;;-----------------------------------------------


(defun cpp/compile-run-buffer ()
  "Compile and run current c++ buffer (single-file C++ code).
Example: By running M-x cpp/compile-buffer-run at current buffer (file code1.cpp)
will run $ g++ code1.cpp -o code1.bin && ./code1.bin"
  (interactive)
  (let* ((file-name  (file-name-nondirectory (buffer-file-name)))
        ;; Change file name extension to .bin 
         (output     (concat (file-name-base file-name) ".bin")))
    (save-buffer)
    (compile (format "g++ %s -o %s && ./%s"
                     file-name
                     output
                     output
                     ))))


(defun cpp/compile-run-dired ()
  "Compile single c++ file at point in dired mode.
Usage: Position the cursor at the file to be compiled and
run M-x cpp/compile-dired
"
  (interactive)
  (let* ((file-name  (file-name-nondirectory (dired-file-name-at-point)))
        ;; Change file name extension to .bin 
         (output     (concat (file-name-base file-name) ".bin")))
    (save-buffer)
    (compile (format "g++ %s -o %s && ./%s"
                     file-name
                     output
                     output
                     ))))


(defun cpp/template-main ()
  "Insert C++ body template at the top of file."
  (interactive)
  (save-excursion           ;; Save cursor position
    (goto-char (point-min)) ;; Got to top of buffer 
    (insert                 ;; Insert template 
     "#include <iostream>
using namespace std;

int main(){

  return 0;

}
" )))

(defun cpp/open-include ()
  "Open /usr/include directory (Linux only)."
  (interactive)
  (dired "/usr/include"))

(defun cpp/open-include-cpp ()
  "Open /usr/include/c++ (Linux only)"
  (interactive)
  (dired "/usr/include/c++"))

(defun cpp/share-libraries ()
  "Show shared libraries in the system. (Linux only)"
  (interactive)
  (shell-command "strings -n5 /etc/ld.so.cache"
                 "*shared-libs*"
                 ))
