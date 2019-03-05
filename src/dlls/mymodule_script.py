import mymodule as a 

print("\n======>>>> Call function: aexposedFunction <<=== ")
a.exposedFunction()

print("\n======>>>> Call function: testArguments <<=== ")

s1 = a.testArguments(False, 100, 10.9361, 0.344, "from Python to C++")
print(" [PYTHON] Received string s1 = ", s1)

s2 = a.testArguments(True, 90, -0.56e6, 9.344e5, "")
print(" [PYTHON]  Received string: s2 = ", s2)

print("\n===== <<<< END OF PYTHON TEST SCRIPT >>>>> =====")
