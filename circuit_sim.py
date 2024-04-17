import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QRadioButton, QHBoxLayout, QGridLayout, QPushButton, QLineEdit
from PyQt6.QtCore import QSize, Qt
from PyQt6.QtGui import QIcon, QPixmap, QTransform
from gui_to_file import initmatrix, write_file
import numpy as np
import os 

global Cvalue
global blocksize
global dimension
global matrix


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.setWindowTitle("Passive Electron Flow Analyser")

        leftPanel = QVBoxLayout()

        self.inputValue = QLineEdit()
        self.inputValue.setMaxLength(5)
        default_text = "100"

        global Cvalue
        Cvalue = int(default_text)
        print(Cvalue)


        self.inputValue.setText(default_text)
        self.inputValue.setPlaceholderText("Enter your Value")
        self.inputValue.returnPressed.connect(self.return_pressed)  # Connect to method in MainWindow
        self.inputValue.textEdited.connect(self.text_edited)



        ### Create radio buttons
        self.resistor_button = QRadioButton("Resistor")
        self.battery_button = QRadioButton("Battery")
        self.wire_button = QRadioButton("Wire")
        self.node_button = QRadioButton("Node")
        
        ## Set "Wire" radio button as default selected
        self.wire_button.setChecked(True)

        ## Connect radio buttons to slot
        self.resistor_button.clicked.connect(self.radio_button_clicked)
        self.battery_button.clicked.connect(self.radio_button_clicked)
        self.wire_button.clicked.connect(self.radio_button_clicked)
        self.node_button.clicked.connect(self.radio_button_clicked)


        ### Adding components to left Panel 
        leftPanel.addWidget(self.inputValue)
        leftPanel.addWidget(self.resistor_button)
        leftPanel.addWidget(self.battery_button)
        leftPanel.addWidget(self.wire_button)
        leftPanel.addWidget(self.node_button)

        ## Create a "Clear" button
        clear_button = QPushButton("Clear")
        clear_button.clicked.connect(self.clear_buttons)
        leftPanel.addWidget(clear_button)

        ## Create a "solve" button
        solve = QPushButton("Solve")
        solve.clicked.connect(self.solve)
        leftPanel.addWidget(solve)

        rightGrid = QGridLayout()
        rightGrid.setSpacing(0)
        rightGrid.setContentsMargins(0, 0, 0, 0)
        
        ## number of column and rows of the grid
        global dimension
        dimension = 5
        n = dimension
        
        ## size in pixel 
        global blocksize
        blocksize = 40

        ## calling initmatrix function from another file to create emptry matrix
        global matrix
        matrix = initmatrix(n)

        self.buttons = []  # List to hold all buttons
        self.button_rotations = {}  # Dictionary to store rotation angle for each button

        for i in range(n):
            for j in range(n):
                button = QPushButton()
                button.setCheckable(True)
                button.setFixedSize(QSize(blocksize, blocksize))
                button.clicked.connect(lambda checked, i=i, j=j: self.button_clicked(i, j))
                rightGrid.addWidget(button, i, j)

                button.setProperty("button_value", n*i + j)
                # Retrieving the value later
                button_value = button.property("button_value")
                # print("Button Value:", button_value)

                self.buttons.append(button)  # Add button to the list
                self.button_rotations[button] = 0  # Initial rotation angle is 0

        outerLayout = QHBoxLayout()
        outerLayout.addLayout(leftPanel)
        outerLayout.addLayout(rightGrid)

        widget = QWidget()
        widget.setLayout(outerLayout)
        self.setCentralWidget(widget)

        ## Keep track of the active component
        self.active_component = "Wire"

    def return_pressed(self):
        # This method will be called when the return key is pressed in the QLineEdit
        text = self.inputValue.text()
        print("Return Pressed:", text)

    def radio_button_clicked(self):
        sender = self.sender()
        if sender.isChecked():
            self.active_component = sender.text()
            print("Active Component:", self.active_component)

    def button_clicked(self, row, col):
        global Cvalue
        global blocksize
        global dimension
        global matrix

        button = self.buttons[row * dimension + col]  # Get the button at the specified row and column
        icon_path = f"./icons/{self.active_component.lower()}.jpg"
        pixmap = QPixmap(icon_path)


        ## Rotate the image
        rotation = self.button_rotations[button]
        # print("button rotation proprty : ",rotation)
        rotation += 90
        if rotation >= 360:
            rotation = 0    
        transform = QTransform().rotate(rotation)
        rotated_pixmap = pixmap.transformed(transform)

        ## Set the rotated icon for the button
        button.setIcon(QIcon(rotated_pixmap))
        button.setIconSize(QSize(blocksize, blocksize))

        ## Update the rotation angle for the button
        self.button_rotations[button] = rotation

        
        val = Cvalue
        # print(self.active_component)
        if self.active_component=='Wire' or self.active_component=='Node':
            val = 0 
        
        if self.active_component=='Wire' or self.active_component=='Resistor':
            if rotation == 180:
                rotation = 0
            if rotation == 270:
                rotation = 90
                

        if self.active_component=='Node':
            rotation = 0

        button_value = button.property("button_value")

        
        match rotation:
            case 0:
                direction = "  ({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col, newrow=row, newcol=col+1)
            case 90:
                direction = " ({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col, newrow=row+1, newcol=col)
            case 180:
                direction = "({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col+1, newrow=row, newcol=col)
            case 270:
                direction = "({row}, {col}) -> ({newrow}, {newcol})".format(row=row+1, col=col, newrow=row, newcol=col)

        n = dimension
        match rotation:
            case 0:
                bval=button_value
                newbval=button_value+1
                edge = "{bval} -> {newbval}".format(bval=button_value, newbval=button_value+1)
            case 90:
                bval=button_value
                newbval=button_value+n
                edge = "{bval} -> {newbval}".format(bval=button_value, newbval=button_value+n)
            case 180:
                bval=button_value+1
                newbval=button_value
                edge = "{bval} -> {newbval}".format(bval=button_value+1, newbval=button_value)
            case 270:
                edge = "{bval} -> {newbval}".format(bval=button_value-n, newbval=button_value)
                bval=button_value-n
                newbval=button_value


        if self.active_component=='Node':
                direction =  "\n({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col, newrow=row, newcol=col+1)
                direction += "\n({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col, newrow=row+1, newcol=col)
                direction += "\n({row}, {col}) -> ({newrow}, {newcol})".format(row=row, col=col+1, newrow=row, newcol=col)
                direction += "\n({row}, {col}) -> ({newrow}, {newcol})".format(row=row+1, col=col, newrow=row, newcol=col)


        ## Adding into Matrix...  
        matrix[bval] = '0'*n

        # matrix[bval*(n*n) + newbval] = 0
        # matrix[newbval*(n*n) + bval] = 0


        if self.active_component=='Wire' or self.active_component=='Battery' or self.active_component=='Resistor':
            matrix[bval*(n*n) + newbval] = 1
            matrix[newbval*(n*n) + bval] = 1
            
        write_file(matrix)
        





        print("Row, Col:", row, col, self.active_component, val, rotation, direction, ' edge :', edge)



    ## Added clear button
    def clear_buttons(self):
        for button in self.buttons:
            button.setChecked(False)
            button.setIcon(QIcon())  # Clear the icon
            self.button_rotations[button] = 0  # Reset rotation angle to 0

        global matrix
        global dimension
        n = dimension
        matrix = initmatrix(n)

        print("Buttons Cleared")


    def return_pressed(self):
        print("Return pressed!")
        self.centralWidget().setText("BOOM!")

    def text_edited(self, s):
        global Cvalue
        Cvalue = s
        value = s
        print('value = ',value)
    
    def solve(self):
        os.system('./a.out')

    




app = QApplication(sys.argv)
window = MainWindow()
window.show()
sys.exit(app.exec())
