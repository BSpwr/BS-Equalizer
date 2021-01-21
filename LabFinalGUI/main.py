from PySide2 import QtCore as Qc
from PySide2 import QtWidgets as Qw
from PySide2 import QtGui as Qg

from equalizer import Equalizer, DiffSurround

from serialManager import SerialManager

import serial
import serial.tools.list_ports
import struct
import timeit
import sys
import signal

class MainWindow(Qw.QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        
        desktop_size = Qg.QGuiApplication.primaryScreen().availableGeometry().size()
        self.resize(desktop_size.width() * 0.4, desktop_size.height() * 0.5)
        
        self.ser_man = SerialManager()
        ports = SerialManager.get_available_ports()
        if len(ports) != 0: self.ser_man.set_port(ports[0])
        
        self.setWindowTitle('BS Equalizer')
        
        self.main_ui = MainUI(parent=self)
        # Set the central widget of the Window.
        self.setCentralWidget(self.main_ui)
        
        self.show()
        
class MainUI(Qw.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
    
        self.equalizer = Equalizer(parent=self)
        self.diff_surround = DiffSurround(parent=self)
        
        self.check_conn_label = Qw.QLabel()
        self.check_conn_label.setAlignment(Qc.Qt.AlignHCenter)
        
        self.check_conn_status = Qw.QPushButton('Check connection status')
        self.check_conn_status.clicked.connect(lambda: self.update_conn_status())
        
        self.conn_port_sel = PortComboBox()
        self.conn_port_sel.activated.connect(self.update_port)
        
        self.conn_status_layout = Qw.QHBoxLayout()
        self.conn_status_layout.addWidget(self.check_conn_label)
        self.conn_status_layout.addWidget(self.check_conn_status)
        self.conn_status_layout.addWidget(self.conn_port_sel)
        
        self.conn_group = Qw.QGroupBox("Connection")
        self.conn_group.setLayout(self.conn_status_layout)
        
        self.equalizer_layout = Qw.QHBoxLayout()
        self.equalizer_layout.addWidget(self.equalizer)
        
        self.equalizer_group = Qw.QGroupBox("Equalizer")
        self.equalizer_group.setLayout(self.equalizer_layout)
        
        self.diff_surround_layout = Qw.QHBoxLayout()
        self.diff_surround_layout.addWidget(self.diff_surround)
        
        self.diff_surround_group = Qw.QGroupBox("Differential Surround")
        self.diff_surround_group.setLayout(self.diff_surround_layout)
        
        self.layout = Qw.QVBoxLayout()
        self.layout.addWidget(self.conn_group)
        self.layout.addWidget(self.equalizer_group)
        self.layout.addWidget(self.diff_surround_group)
        self.setLayout(self.layout)
        
        self.update_conn_status()
        
        self.show()
        
    def update_port(self, port_idx: str):
        self.parent().ser_man.set_port(self.conn_port_sel.itemText(port_idx))
        self.update_conn_status()
    
    def update_conn_status(self):
        ports = SerialManager.get_available_ports()
        if self.parent().ser_man.ser.port in ports and self.parent().ser_man.check_conn():
            self.check_conn_label.setText("DSP Connected ✔️")
            # load values from DSP
            self.equalizer.load_settings()
            self.diff_surround.load_settings()
        else:
            self.check_conn_label.setText("DSP Disconnected ✖️")

class PortComboBox(Qw.QComboBox):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.updatePorts()
            
    def showPopup(self):
        self.updatePorts()
        super().showPopup()
        
    def updatePorts(self):
        self.clear()
        ports = SerialManager.get_available_ports()
        for port in ports:
            self.addItem(port)

def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    
    app = Qw.QApplication(sys.argv)  # Create application
    app.setPalette(app.style().standardPalette())
    window = MainWindow()  # Create main window
    
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
