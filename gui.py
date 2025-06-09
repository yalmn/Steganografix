# Simple GUI for Steganografix using tkinter
import os
import subprocess
import tkinter as tk
from tkinter import filedialog, messagebox


def select_input_file(entry):
    path = filedialog.askopenfilename()
    if path:
        entry.delete(0, tk.END)
        entry.insert(0, path)


def select_payload_file(entry):
    path = filedialog.askopenfilename()
    if path:
        entry.delete(0, tk.END)
        entry.insert(0, path)


def select_output_dir(entry):
    path = filedialog.askdirectory()
    if path:
        entry.delete(0, tk.END)
        entry.insert(0, path)


def run_steganografix(mode_var, input_entry, payload_entry, key_entry, enc_var, out_dir_entry):
    binary = os.path.join(os.path.dirname(__file__), 'steganografix')
    if not os.path.isfile(binary):
        messagebox.showerror('Error', 'steganografix binary not found. Build it with make first.')
        return

    mode = mode_var.get()
    input_file = input_entry.get()
    key = key_entry.get()
    enc = enc_var.get()
    out_dir = out_dir_entry.get()

    if not input_file or not key or not out_dir:
        messagebox.showerror('Error', 'Please select input file, key and output directory.')
        return

    cmd = [binary]
    if mode == 'embed':
        payload_file = payload_entry.get()
        if not payload_file:
            messagebox.showerror('Error', 'Please select payload file for embedding.')
            return
        base = os.path.basename(input_file)
        output_file = os.path.splitext(base)[0] + '_stego' + os.path.splitext(base)[1]
        output_path = os.path.join(out_dir, output_file)
        cmd += ['embed', input_file, payload_file, output_path]
    else:
        base = os.path.basename(input_file)
        output_file = os.path.splitext(base)[0] + '_extracted'
        output_path = os.path.join(out_dir, output_file)
        cmd += ['extract', input_file, output_path]

    cmd.append(f'--key={key}')
    if enc == 'aes':
        cmd.append('--aes')
    else:
        cmd.append('--xor')

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        messagebox.showinfo('Success', result.stdout or 'Operation completed.')
    except subprocess.CalledProcessError as e:
        messagebox.showerror('Error', e.stderr or 'Operation failed.')


def build_gui():
    root = tk.Tk()
    root.title('Steganografix GUI')

    mode_var = tk.StringVar(value='embed')
    enc_var = tk.StringVar(value='xor')

    # Mode selection
    tk.Label(root, text='Mode:').grid(row=0, column=0, sticky='w')
    tk.Radiobutton(root, text='Embed', variable=mode_var, value='embed').grid(row=0, column=1, sticky='w')
    tk.Radiobutton(root, text='Extract', variable=mode_var, value='extract').grid(row=0, column=2, sticky='w')

    # Input file
    tk.Label(root, text='Input file:').grid(row=1, column=0, sticky='w')
    input_entry = tk.Entry(root, width=40)
    input_entry.grid(row=1, column=1, columnspan=2, sticky='we')
    tk.Button(root, text='Browse', command=lambda: select_input_file(input_entry)).grid(row=1, column=3, sticky='w')

    # Payload file (only for embed)
    tk.Label(root, text='Payload file:').grid(row=2, column=0, sticky='w')
    payload_entry = tk.Entry(root, width=40)
    payload_entry.grid(row=2, column=1, columnspan=2, sticky='we')
    tk.Button(root, text='Browse', command=lambda: select_payload_file(payload_entry)).grid(row=2, column=3, sticky='w')

    # Key
    tk.Label(root, text='Key:').grid(row=3, column=0, sticky='w')
    key_entry = tk.Entry(root, width=40)
    key_entry.grid(row=3, column=1, columnspan=2, sticky='we')

    # Encryption type
    tk.Label(root, text='Encryption:').grid(row=4, column=0, sticky='w')
    tk.Radiobutton(root, text='XOR', variable=enc_var, value='xor').grid(row=4, column=1, sticky='w')
    tk.Radiobutton(root, text='AES', variable=enc_var, value='aes').grid(row=4, column=2, sticky='w')

    # Output directory
    tk.Label(root, text='Output directory:').grid(row=5, column=0, sticky='w')
    out_dir_entry = tk.Entry(root, width=40)
    out_dir_entry.grid(row=5, column=1, columnspan=2, sticky='we')
    tk.Button(root, text='Browse', command=lambda: select_output_dir(out_dir_entry)).grid(row=5, column=3, sticky='w')

    # Run button
    tk.Button(root, text='Run', command=lambda: run_steganografix(mode_var, input_entry, payload_entry, key_entry, enc_var, out_dir_entry)).grid(row=6, column=1, pady=10)

    root.mainloop()


if __name__ == '__main__':
    build_gui()
