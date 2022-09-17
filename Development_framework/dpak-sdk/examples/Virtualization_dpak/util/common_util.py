import subprocess

def subprocess_command(ssh_command):
    proc = str(subprocess.Popen(ssh_command, shell=False, stdout=subprocess.PIPE,stderr=subprocess.PIPE).stdout.read(),
                encoding='utf-8').strip()
    return proc

def common_result_check_contain(software, value, check_item):
    result = False
    if value == 'fail':
        message = '{} check fail,{}'.format(software, value)
        return result, message
    
    for item in check_item:
        if item in value:
            message = '{} is ok'.format(software)
            return True, message
    
    message = '{} is not ok,{}'.format(software, value)
    return False, message
                

def common_result_check_not_contain(software, value, check_item):
    result = False
    if value == 'fail':
        message = '{} check fail,{}'.format(software, value)
        return result, message
    
    for item in check_item:
        if item not in value:
            message = '{} is ok'.format(software)
            return True, message
    
    message = '{} is not ok,{}'.format(software, value)
    return False, message

def findinfor(echo, image, k):
    echo = echo.split(' ')
    for i in range(len(echo) -1, -1, -1):
        if echo[i] == "" or echo[i] == "|" or echo[i] == "|\r\n|":
            del echo[i]
    for i, _x in enumerate(echo):
        if echo[i] == image:
            information = echo[i + k]
            break
    return information