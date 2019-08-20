import lib.cmdItf as cmdItf
from PyQt5.QtCore import QObject, pyqtSlot

class Interaction(QObject):
	@pyqtSlot()
	def cmdInit(self):
		cmdItf.init()

	@pyqtSlot(int, int, int, int, bool, int, bool, bool, int)
	def updateCommands(self, rbtID, vx, vy, vr, dribble, dribbleLevel, mode, shoot, power):
		if dribble:
			cmdItf.setSpeed(rbtID, dribbleLevel, vx, vy, vr)
		else:
			cmdItf.setSpeed(rbtID, 0, vx, vy, vr)
		if not shoot:
			cmdItf.setKick(rbtID, 0, 0)
		else:
			if not mode:
				cmdItf.setKick(rbtID, power, 0)
			else:
				cmdItf.setKick(rbtID, 0, power)
	
	@pyqtSlot()
	def sendCommands(self):
		cmdItf.sendCommands()
