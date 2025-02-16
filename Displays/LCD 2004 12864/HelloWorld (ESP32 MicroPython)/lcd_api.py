class LcdApi:
    def __init__(self, num_lines, num_columns):
        self.num_lines = num_lines
        self.num_columns = num_columns
        self.cursor_x = 0
        self.cursor_y = 0
    
    def clear(self):
        self._write_command(0x01)
        self.cursor_x = 0
        self.cursor_y = 0
    
    def move_to(self, x, y):
        self.cursor_x = x
        self.cursor_y = y
        addr = 0x80 + x + (0x40 * y)
        self._write_command(addr)
    
    def putchar(self, char):
        self._write_data(ord(char))
    
    def putstr(self, string):
        for char in string:
            self.putchar(char)
    
    def _write_command(self, cmd):
        raise NotImplementedError
    
    def _write_data(self, data):
        raise NotImplementedError