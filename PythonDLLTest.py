import ctypes
import time
# Load the shared library
mydll = ctypes.CDLL('./libgccDppConsoleMiniX2.so')
result = mydll.ConnectToDefaultDPP()
test = mydll.GetDppStatus()

print('Interlock Status')
test2 = mydll.GetInterlockStatus()


test3 = mydll.ReadHVCfg()


print('Turning On')
on = mydll.TurnHVOff()

test4 = mydll.ReadHVCfg()



# mydll = ctypes.CDLL('./libmydll.so')


# Define the function prototype
# mydll.add.argtypes = (ctypes.c_int, ctypes.c_int)
# mydll.add.restype = ctypes.c_int

# # Call the function
#result = mydll.ConnectToDefaultDPP()
# result = mydll.add(3, 4)
# print(f"The result of add(3, 4) is {result}")