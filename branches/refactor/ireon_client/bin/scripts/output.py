import sys
import App
##### Standart output redirect
sys.stderr = App.Output()
sys.stdout = App.Output()
