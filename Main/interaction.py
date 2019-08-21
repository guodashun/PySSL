import cmdIf
from PyQt5.QtCore import QObject, pyqtSlot

class Interaction(QObject):
	@pyqtSlot()
	def cmdInit(self):
		cmdIf.init()

	@pyqtSlot(int, int, int, int, bool, int, bool, bool, int)
	def updateCommands(self, rbtID, vx, vy, vr, dribble, dribbleLevel, mode, shoot, power):
		if dribble:
			cmdIf.setSpeed(rbtID, dribbleLevel, vx, vy, vr)
		else:
			cmdIf.setSpeed(rbtID, 0, vx, vy, vr)
		if not shoot:
			cmdIf.setKick(rbtID, 0, 0)
		else:
			if not mode:
				cmdIf.setKick(rbtID, power, 0)
			else:
				cmdIf.setKick(rbtID, 0, power)
	
	@pyqtSlot()
	def sendCommands(self):
		cmdIf.sendCommands()
