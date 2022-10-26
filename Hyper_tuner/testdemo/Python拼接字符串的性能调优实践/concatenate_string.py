import sys

BASE_STRING = "Hello world"
LOOP_TIMES = 100000000
STRING_LIST = [BASE_STRING] * LOOP_TIMES

def string_plus():
	new_string = ''
	for string in STRING_LIST:
		new_string += string
	print(len(new_string))
	

def string_join():
	new_string = ''.join(STRING_LIST)
	print(len(new_string))
	

def main():
	function_map = {
		'string_plus': string_plus,
		'string_join': string_join,
	}
	print(sys.argv)
	_, function = sys.argv
	if function in function_map:
		function_map[function]()
	else:
		print('function {} not exist'.format(function))
		
		
if __name__ == "__main__":
	main()
	