import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QRadioButton, QHBoxLayout, QGridLayout, QPushButton, QLineEdit
from PyQt6.QtCore import QSize, Qt
from PyQt6.QtGui import QIcon, QPixmap, QTransform
from gui_to_file import initmatrix, insertdata, remove_zero_rows_and_cols, write_into_files, add_zero_row_and_column
import os 
import numpy as np

global Cvalue
global blocksize
global dimension
global nodedimension
global button_edge_set


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
        print("default value :",Cvalue)


        self.inputValue.setText(default_text)
        self.inputValue.setPlaceholderText("Enter your Value")
        self.inputValue.returnPressed.connect(self.return_pressed)  # Connect to method in MainWindow
        self.inputValue.textEdited.connect(self.text_edited)



        ### Create radio buttons
        self.resistor_button = QRadioButton("Resistor")
        self.battery_button = QRadioButton("Battery")
        self.wire_button = QRadioButton("Wire")
        # self.node_button = QRadioButton("Node")
        self.erase_button = QRadioButton("Erase")
        
        ## Set "Wire" radio button as default selected
        self.wire_button.setChecked(True)

        ## Connect radio buttons to slot
        self.resistor_button.clicked.connect(self.radio_button_clicked)
        self.battery_button.clicked.connect(self.radio_button_clicked)
        self.wire_button.clicked.connect(self.radio_button_clicked)
        # self.node_button.clicked.connect(self.radio_button_clicked)
        self.erase_button.clicked.connect(self.radio_button_clicked)


        ### Adding components to left Panel 
        leftPanel.addWidget(self.inputValue)
        leftPanel.addWidget(self.resistor_button)
        leftPanel.addWidget(self.battery_button)
        leftPanel.addWidget(self.wire_button)
        # leftPanel.addWidget(self.node_button)
        leftPanel.addWidget(self.erase_button)

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
        global nodedimension
        nodedimension = 8
        dimension = nodedimension*2 - 1
        n = dimension
        
        ## size in pixel 
        global blocksize
        blocksize = 50


        self.buttons = []  # List to hold all buttons
        self.button_rotations = {}  # Dictionary to store rotation angle for each button


        blocksize = 50



        #### layout 

        ### i even j even - node
        ### i even j odd  - horizontal component
        ### i odd  j even - verticle component
        ### i odd  j odd  - block

        for i in range(n):
            if i%2 == 0:
                for j in range(n):
                    button = QPushButton()
                    if j%2 != 0:
                        ### i even j odd
                        button.setCheckable(True)
                        button.setFixedSize(QSize(int(blocksize), int(blocksize/2)))
                    else:
                        ### i even j even
                        button.setCheckable(True)
                        button.setFixedSize(QSize(int(blocksize/2), int(blocksize/2)))
                
                    button.clicked.connect(lambda checked, i=i, j=j: self.button_clicked(i, j))
                    # print(i, j)
                    rightGrid.addWidget(button, i, j)

                    button.setProperty("button_name", n*i + j)
                    # Retrieving the value later
                    button_name = button.property("button_name")
                    # print("Button Value:", button_name)
                    self.buttons.append(button)  # Add button to the list
                    self.button_rotations[button] = 0  # Initial rotation angle is 0
            else:
                for j in range(n):
                    button = QPushButton()
                    if j%2 != 0:
                        ### i odd j odd
                        button.setCheckable(True)
                        button.setFixedSize(QSize(int(blocksize), int(blocksize)))
                    else:
                        ### i odd j even
                        button.setCheckable(True)
                        button.setFixedSize(QSize(int(blocksize/2), int(blocksize)))

                    button.clicked.connect(lambda checked, i=i, j=j: self.button_clicked(i, j))
                    # print(i, j)
                    rightGrid.addWidget(button, i, j)

                    button.setProperty("button_name", n*i + j)
                    # Retrieving the value later
                    button_name = button.property("button_name")
                    # print("Button Value:", button_name)
                    self.buttons.append(button)  # Add button to the list
                    self.button_rotations[button] = 90  # Initial rotation angle is 0
                        

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

        # print(row * dimension + col)
        button = self.buttons[row * dimension + col]  # Get the button at the specified row and column

        if self.active_component == 'Erase':
            button.setChecked(False)
            button.setIcon(QIcon())  # Clear the icon
            return

        if row%2!=0 and col%2!=0:
            button.setChecked(True)
            icon_path = f"./icons/blank.jpg"
            pixmap = QPixmap(icon_path)
            button.setIcon(QIcon(pixmap))
            button.setIconSize(QSize(blocksize, blocksize))
            self.button_rotations[button] = 0
            return
        
        
        icon_path = f"./icons/{self.active_component.lower()}.jpg"
        if row%2==0 and col%2==0:
            icon_path = "./icons/node.jpg"
            val = 0
        pixmap = QPixmap(icon_path)


        ## Rotate the image
        rotation = self.button_rotations[button]
        # print("button rotation proprty : ",rotation)
        rotation += 180
        if rotation >= 360:
            rotation -= 360    
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

        button.setProperty("button_value", float(val))
        button.setProperty("button_rotation", rotation)
        button.setProperty("button_row", row)
        button.setProperty("button_col", col)
        button.setProperty("button_component", self.active_component)

        button_name = button.property("button_name")
        button_value = button.property("button_value")
        button_rotation = button.property("button_rotation")
        button_row = button.property("button_row")
        button_col = button.property("button_col")
        button_component = button.property("button_component")

        #### layout reminder

        ### i even j even - node
        ### i even j odd  - horizontal component
        ### i odd  j even - verticle component
        ### i odd  j odd  - block

        edge = '-----'
        n = dimension
        button.setProperty("button_edge_from", None)
        button.setProperty("button_edge_to", None)
        node1 = None
        node2 = None

        if (button_row%2!=0 and button_col%2==0):
            if button_rotation == 90:
                node1 = button_name - n
                node2 = button_name + n
            if button_rotation == 270:
                node1 = button_name + n
                node2 = button_name - n
            edge = "{node1}->{node2}".format(node1 = node1, node2 = node2)
            button.setProperty("button_edge_from", node1)
            button.setProperty("button_edge_to", node2)
        if (button_row%2==0 and button_col%2!=0):
            if button_rotation == 0:
                node1 = button_name - 1
                node2 = button_name + 1
            if button_rotation == 180:
                node1 = button_name + 1
                node2 = button_name - 1
            edge = "{node1}->{node2}".format(node1 = node1, node2 = node2)
            button.setProperty("button_edge_from", node1)
            button.setProperty("button_edge_to", node2)
        



        print(button_component,'\t', button_name,'\t', button_value,'\t', button_rotation,'\t', button_row,'\t', button_col,'\t', edge)
        return




    ## Added clear button
    def clear_buttons(self):
        for button in self.buttons:
            button.setChecked(False)
            button.setIcon(QIcon())  # Clear the icon

        global dimension
        n = dimension

        global button_edge_set
        button_edge_set = []

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
        global button_edge_set
        button_edge_set = []
        data = None

        
        for button in self.buttons:
            button_name = button.property("button_name")
            button_value = button.property("button_value")
            button_rotation = button.property("button_rotation")
            button_row = button.property("button_row")
            button_col = button.property("button_col")
            button_component = button.property("button_component")
            button_edge_from = button.property("button_edge_from")
            button_edge_to = button.property("button_edge_to")
            # print(button_component, button_name, button_value, button_rotation, button_row, button_col, button_edge_from, button_edge_to)
            
            ## params passing lists
            element = [button_component, button_value, button_edge_from, button_edge_to]
            button_edge_set.append(element)

        
        ### calling file writing functions

        #init matrix
        global dimension
        matrix_dimension= dimension*dimension
        matrix, Rmatrix, Vmatrix = initmatrix(matrix_dimension)

        data = [matrix, Rmatrix, Vmatrix]

        # print(button_edge_set)
        # print(matrix)


        # put values in those matrices
        data = insertdata(data, button_edge_set)

        #slice the three matrices
        data = remove_zero_rows_and_cols(data)




        #### adding empty row and empty column as per request by 
        #### Tejas Joshi , Soham Haldankar, Tejas Kolhe
        #### for C program
        data[0] = add_zero_row_and_column(data[0])
        data[1] = add_zero_row_and_column(data[1])
        data[2] = add_zero_row_and_column(data[2])

        

        print("\nprinting adjacency matrix...")
        print(data[0])
        print("\nprinting resistor matrix...")
        print(data[1])
        print("\nprinting voltage matrix...")
        print(data[2])

        # writing all the necessary files
        write_into_files(data)

        # executing C program
        os.system('chmod + execute.sh')
        os.system('./execute.sh')



        





### program starts

app = QApplication(sys.argv)
window = MainWindow()
window.show()
sys.exit(app.exec())
