import tkinter as tk
from tkinter import filedialog, messagebox, Menu
from tkinter import ttk
import subprocess
import threading
import os
import sys

class BrailleTranslatorUI:
    def __init__(self, master):
        self.master = master
        master.title("Braille Translator")
        master.geometry("700x600")

        # Create menu bar
        self.create_menu()

        # Create main frames
        self.create_widgets()

    def create_menu(self):
        menu_bar = Menu(self.master)
        self.master.config(menu=menu_bar)

        file_menu = Menu(menu_bar, tearoff=0)
        file_menu.add_command(label="Open File...", command=self.upload_file)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.master.quit)
        menu_bar.add_cascade(label="File", menu=file_menu)

        help_menu = Menu(menu_bar, tearoff=0)
        help_menu.add_command(label="About", command=self.show_about)
        menu_bar.add_cascade(label="Help", menu=help_menu)

    def create_widgets(self):
        # Input Frame
        input_frame = ttk.LabelFrame(self.master, text="Input Text")
        input_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        self.text_input = tk.Text(input_frame, height=8)
        self.text_input.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        input_button_frame = ttk.Frame(input_frame)
        input_button_frame.pack(fill=tk.X, padx=5, pady=5)

        self.translate_button = ttk.Button(input_button_frame, text="Translate", command=self.translate_text)
        self.translate_button.pack(side=tk.LEFT, padx=(0, 5))

        self.upload_button = ttk.Button(input_button_frame, text="Upload File", command=self.upload_file)
        self.upload_button.pack(side=tk.LEFT, padx=5)

        self.clear_input_button = ttk.Button(input_button_frame, text="Clear Input", command=lambda: self.text_input.delete("1.0", tk.END))
        self.clear_input_button.pack(side=tk.LEFT, padx=5)

        # Output Frame
        output_frame = ttk.LabelFrame(self.master, text="Braille Output")
        output_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        self.output_area = tk.Text(output_frame, height=12)
        self.output_area.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        output_button_frame = ttk.Frame(output_frame)
        output_button_frame.pack(fill=tk.X, padx=5, pady=5)

        self.save_button = ttk.Button(output_button_frame, text="Save Output", command=self.save_output)
        self.save_button.pack(side=tk.LEFT, padx=(0, 5))

        self.clear_output_button = ttk.Button(output_button_frame, text="Clear Output", command=lambda: self.output_area.delete("1.0", tk.END))
        self.clear_output_button.pack(side=tk.LEFT, padx=5)

    def translate_text(self):
        text = self.text_input.get("1.0", tk.END).strip().lower()
        if text:
            self.run_c_program(text)
        else:
            messagebox.showwarning("Input Needed", "Please enter some text to translate.")

    def upload_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if file_path:
            try:
                with open(file_path, 'r') as file:
                    content = file.read().strip().lower()
                    self.text_input.delete("1.0", tk.END)
                    self.text_input.insert(tk.END, content)
            except Exception as e:
                messagebox.showerror("File Error", f"An error occurred while opening the file:\n{e}")

    def run_c_program(self, input_text):
        # Disable the translate button to prevent multiple clicks
        self.translate_button.config(state=tk.DISABLED)

        if not os.path.exists("braille_translator"):
            compile_result = subprocess.run(["gcc", "-o", "braille_translator", "braille_translator.c"], capture_output=True, text=True)
            if compile_result.returncode != 0:
                self.output_area.delete("1.0", tk.END)
                self.output_area.insert(tk.END, f"Compilation error:\n{compile_result.stderr}")
                self.translate_button.config(state=tk.NORMAL)
                return

        # Run the C program in a separate thread
        threading.Thread(target=self.run_and_capture_output, args=(input_text,)).start()

    def run_and_capture_output(self, input_text):
        try:
            # Start the subprocess
            process = subprocess.Popen(["./braille_translator", input_text],
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE,
                                       text=True)

            # Read output line by line
            for line in iter(process.stdout.readline, ''):
                if line:
                    # Update the output area in the main thread
                    self.output_area.after(0, self.append_output, line)
            # Wait for the process to finish
            process.stdout.close()
            process.wait()

            # Check for any errors
            stderr_output = process.stderr.read()
            if stderr_output:
                self.output_area.after(0, self.append_output, f"\nErrors:\n{stderr_output}")

        except Exception as e:
            messagebox.showerror("Execution Error", f"An error occurred while running the translator:\n{e}")
        finally:
            # Re-enable the translate button
            self.output_area.after(0, lambda: self.translate_button.config(state=tk.NORMAL))

    def append_output(self, text):
        self.output_area.insert(tk.END, text)
        self.output_area.see(tk.END)  # Scroll to the end

    def save_output(self):
        output_text = self.output_area.get("1.0", tk.END).strip()
        if output_text:
            file_path = filedialog.asksaveasfilename(defaultextension=".txt",
                                                     filetypes=[("Text Files", "*.txt")])
            if file_path:
                try:
                    with open(file_path, 'w') as file:
                        file.write(output_text)
                    messagebox.showinfo("Success", "Output saved successfully.")
                except Exception as e:
                    messagebox.showerror("Save Error", f"An error occurred while saving the file:\n{e}")
        else:
            messagebox.showwarning("No Output", "There is no output to save.")

    def show_about(self):
        messagebox.showinfo("About Braille Translator", "Braille Translator v1.0\nDeveloped with Tkinter.")

if __name__ == "__main__":
    root = tk.Tk()
    app = BrailleTranslatorUI(root)
    root.mainloop()