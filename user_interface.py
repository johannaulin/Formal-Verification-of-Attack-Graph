#!/usr/bin/env python3

import tkinter as tk
from tkinter import *
from tkinter import filedialog
from tkinter import ttk
from tkinter.messagebox import showerror
from threading import Thread

import subprocess, os, sys
from subprocess import PIPE

class RunAlgorithm(Thread):
    def __init__(self, attackpath, countermeasures, graphmodel):
        super().__init__()

        self.attackpath = attackpath
        self.countermeasures = countermeasures
        self.graphmodel = graphmodel

    def run(self):    	
    	os.environ["PYTHONUNBUFFERED"] = "1"
    	subprocess.call("gcc main.c -o exc", shell = True)
    	
    	subprocess.call("./exc " + self.attackpath + " " + self.countermeasures + " " + self.graphmodel, shell=True)
    	
    	output = subprocess.run("./mcmas -a ISPL_file.ispl", shell=True, stdout=PIPE)
    	#print("".join([x for x in output.stdout.decode().split("\n") if "Formula number" in x]))
    	
    	ret = [x for x in output.stdout.decode().split("\n") if "Formula number" in x]
    	return ret

specifications = []
class App(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title('User interface')
        self.geometry('680x430')
        self.resizable(0, 0)

        self.create_header_frame()
        self.create_body_frame()
        self.create_footer_frame()
    
    def openFile(self):
        tf = filedialog.askopenfilename()
        tf_stripped = tf.split('/')[-1]
        if tf_stripped not in specifications:
            specifications.append(tf_stripped)
        self.text.insert(END, "You have selected " + tf_stripped + "\n")
        if len(specifications) == 3:
            self.text.insert(END, "\n\nYou have selected three files. Click Execute to see results of attack graph generation.")
        #tf = open(tf)  # or tf = open(tf, 'r')
        #data = tf.read()
        #tf.close()

    def create_header_frame(self):

        self.header = ttk.Frame(self)
        # configure the grid
        self.header.columnconfigure(0, weight=1)
        self.header.columnconfigure(1, weight=10)
        self.header.columnconfigure(2, weight=1)
        # label
        self.label = ttk.Label(self.header, text='Please upload the required files in the format: attack graph, countermeasures, game model.')
        self.label.grid(column=0, row=0, sticky=tk.W)

        # execute button
        self.execute_button = ttk.Button(self.header, text='Execute')
        self.execute_button['command'] = self.handle_download
        self.execute_button.grid(column=2, row=0, sticky=tk.E)

        # attach the header frame
        self.header.grid(column=0, row=0, sticky=tk.NSEW, padx=10, pady=10)

    def handle_download(self):
        if len(specifications) == 3:
            self.execute_button['state'] = tk.DISABLED

            attackpath, countermeasures, graphmodel = specifications[0], specifications[1], specifications[2]
            download_thread = RunAlgorithm(attackpath, countermeasures, graphmodel)
            output = download_thread.run()
            
            self.text.insert(END, "\n\nThe output of your model is:\n")
            for out in output:
            	self.text.insert(END, out + "\n")
            
            self.text.insert(END, "\n\t*****************************\n\nThank you for using our model. If you want to change your specifications, please click on the Exit button to quit the current program and then restart the process with your new files. Thank you.")
            
        else:
            showerror(title='Error',
                      message='You have not provided all the required files.')

    def create_body_frame(self):
        self.body = ttk.Frame(self)
        # text and scrollbar
        self.text = tk.Text(self.body, height=20)
        self.text.grid(column=0, row=1)

        scrollbar = ttk.Scrollbar(self.body,
                                  orient='vertical',
                                  command=self.text.yview)

        scrollbar.grid(column=1, row=1, sticky=tk.NS)

        openfile = Button(self.body, text="Open File", command=self.openFile)
        openfile.grid(column=2, row=2, sticky=tk.NS)

        # attach the body frame
        self.body.grid(column=0, row=1, sticky=tk.NSEW, padx=10, pady=10)
    
    def create_footer_frame(self):
        self.footer = ttk.Frame(self)
        # configure the grid
        self.footer.columnconfigure(0, weight=1)
        # exit button
        self.exit_button = ttk.Button(self.footer,
                                      text='Exit',
                                      command=self.destroy)

        self.exit_button.grid(column=0, row=0, sticky=tk.E)

        # attach the footer frame
        self.footer.grid(column=0, row=2, sticky=tk.NSEW, padx=10, pady=10)

if __name__ == "__main__":
    app = App()
    app.mainloop()
