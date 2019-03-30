import sys
import string
import enum
import copy
import readline
import rlcompleter

class Noreturn:
    pass

class Nil:
    pass

noreturn = Noreturn()

def slurp(name):
    io = open(name)
    ret = io.read()
    io.close()
    return ret

class Token:
    def __init__(self, tok, kind):
        self.tok = tok
        self.kind = kind
    def __str__(self):
        return f'Token(\'{self.tok}\', \'{self.kind}\')'
    def __repr__(self):
        return str(self)

namestr = string.ascii_letters + '_'
namestrtail = namestr + string.digits

numberstr = string.digits
numberstrtail = string.digits + '.'

braces = [
    '{', '}',
    '[', ']',
]

def tokenize(code):
    lencode = len(code)
    pl = 0
    ret = []
    while pl < lencode:
        cur = code[pl]
        if cur in namestr:
            tok = ''
            while cur in namestrtail:
                tok += cur
                pl += 1
                if pl >= lencode:
                    break
                cur = code[pl]
            ret.append(Token(tok, 'str'))
        elif cur == '$':
            tok = ''
            begin = True
            while cur in namestrtail or begin:
                if begin:
                    begin = False
                else:
                    tok += cur
                pl += 1
                if pl >= lencode:
                    break
                cur = code[pl]
            ret.append(Token(tok, 'name'))
        elif cur in numberstr:
            tok = ''
            while cur in numberstrtail:
                tok += cur
                pl += 1
                if pl >= lencode:
                    break
                cur = code[pl]
            ret.append(Token(float(tok) if '.' in tok else int(tok), 'number'))
        elif cur == '(':
            depth = 0
            tok = ''
            while True:
                cur = code[pl]
                if depth != 0:
                    tok += cur
                if cur == '(':
                    depth += 1
                if cur == ')':
                    depth -= 1
                pl += 1
                if pl >= lencode or depth == 0:
                    break
            tok = tok[:-1]
            ret.append(Token(tok, 'str'))
        elif cur in '\n\r':
            ret.append(Token('newline', 'newline'))
            pl += 1
        elif cur in ';':
            ret.append(Token('break', 'newline'))
            pl += 1
        elif cur in braces:
            ret.append(Token(cur, 'brace'))
            pl += 1
        elif cur in '\t ':
            pl += 1
        else:
            print('error: cannot parse')
            exit()
    return ret

def mate(tokens):
    ret = {}
    depth = 0
    hold = None
    for pl, i in enumerate(tokens):
        if i.kind == 'brace':
            if i.tok in '[{':
                if depth == 0:
                    hold = pl
                depth += 1
            if i.tok in ']}':
                depth -= 1
                if depth == 0:
                    ret[hold] = pl
                    ret[pl] = hold
    return ret

def split(tokens):
    ret = []
    current = []
    for i in tokens:
        if isinstance(i, Token) and i.kind == 'newline':
            if current != []:
                ret.append(current)
            current = []
        else:
            current.append(i)
    if current != []:
        ret.append(current)
    return ret

def clump(tokens):
    mates = mate(tokens)
    ret = []
    pl = 0
    lenmates = len(tokens)
    while pl < lenmates:
        if pl in mates:
            subtoks = tokens[pl+1:mates[pl]]
            if tokens[pl].tok == '{':
                clumped = clump(subtoks)
                lines = split(clumped)
                ret.append(('proc', lines))
                pl = mates[pl]
            else:
                clumped = clump(subtoks)
                lines = split(clumped)
                ret.append(lines)
                pl = mates[pl]
        else:
            ret.append(tokens[pl])
        pl += 1
    return ret

wlocals = {}

def fn_set(info, name, value):
    wlocals[-1][name] = value

def fn_add(info, *args):
    ret = 0
    for i in args:
        ret += i
    return ret

def fn_sub(info, fst, *args):
    ret = fst
    for i in args:
        ret -= i
    return ret

def fn_list(info, *args):
    return list(args)

def fn_mul(info, *args):
    result = 1
    for i in args:
        result *= i
    return result

def fn_div(info, fst, *args):
    result = fst
    for i in args:
        result /= i
    return result

def fn_mod(info, fst, *args):
    result = fst
    for i in args:
        result %= i
    return result

def fn_setind(info, lis, ind, to):
    lis[ind] = to
    return lis

def fn_proc(info, name, *args):
    num = args[-1]
    args = args[:-1]
    def fn(info, *fnargs):
        info.calls.append(info.pl)
        wlocals.append({})
        for k, v in zip(args, fnargs):
            wlocals[-1][k] = v
        info.pl = num
        return noreturn
    wlocals[-1][name] = fn

def fn_if(info, check, ifv, elsev=None):
    if check is not nil and check is not False:
        func = ifv
    elif elsev is None:
        return
    else:
        func = elsev
    info.call(func)
    return noreturn
def fn_lte(info, x, y):
    return x <= y
def fn_gt(info, x, y):
    return x > y
def fn_gte(info, x, y):
    return x >= y
def fn_eq(info, x, y):
    return x == y
def fn_neq(info, x, y):
    return x != y
def fn_lt(info, x, y):
    return x < y

def fn_print(info, *args):
    print(*args)

def fn_exit(info, val=0):
    exit(val)

def fn_copy(info, x):
    return copy.deepcopy(x)

nil = Nil()

wbuiltins = {}
wbuiltins['proc'] = fn_proc
wbuiltins['if'] = fn_if
wbuiltins['set'] = fn_set
wbuiltins['add'] = fn_add
wbuiltins['sub'] = fn_sub
wbuiltins['mul'] = fn_mul
wbuiltins['div'] = fn_div
wbuiltins['mod'] = fn_mod
wbuiltins['lt'] = fn_lt
wbuiltins['lte'] = fn_lte
wbuiltins['gt'] = fn_gt
wbuiltins['gte'] = fn_gte
wbuiltins['eq'] = fn_eq
wbuiltins['neq'] = fn_neq
wbuiltins['print'] = fn_print
wbuiltins['none'] = nil
wbuiltins['copy'] = fn_copy
wbuiltins['true'] = True
wbuiltins['false'] = False
wbuiltins['list'] = fn_list
wbuiltins['exit'] = fn_exit

wlocals = [wbuiltins, {}]

def get(n):
    if n in wlocals[0]:
        return wlocals[0][n]
    for cur in wlocals[::-1]:
        if n in cur:
            return cur[n]
    raise NameError(f'no name \'{n}\'')

class Vm:

    def bytecode(self, ast):
        ret = []
        if isinstance(ast, list):
            for cmd in ast:
                ret.append('name')
                ret.append(cmd[0].tok)
                for i in cmd[1:]:
                    ret += self.bytecode(i)
                ret.append('call')
                ret.append(len(cmd)-1)
                ret.append('pop')
            if len(ret) > 0:
                ret.pop()
        elif isinstance(ast, tuple):
            ret.append('proc')
            fn = self.bytecode(ast[1])
            fn.append('ret')
            ret.append(len(fn))
            ret += fn
        elif isinstance(ast, Token):
            if ast.kind == 'number':
                ret.append('num')
                ret.append(ast.tok)
            if ast.kind == 'name':
                ret.append('name')
                ret.append(ast.tok)
            if ast.kind == 'str':
                ret.append('str')
                ret.append(ast.tok)
        return ret

    def call(self, fn, *args):
        if callable(fn):
            got = fn(self, *args)
            if got is not noreturn:
                self.stack.append(got)
        elif isinstance(fn, int):
            wlocals.append({})
            self.calls.append(self.pl)
            self.pl = fn

    def vmrun(self, code):
        self.pl = 0
        self.code = code
        end = len(code)
        self.stack = [None]
        self.calls = []
        while self.pl < end:
            if code[self.pl] == 'name':
                self.stack.append(get(code[self.pl+1]))
                self.pl += 1
            elif code[self.pl] == 'ret':
                self.pl = self.calls[-1] + 1
                self.calls.pop()
                wlocals.pop()
            elif code[self.pl] == 'pop':
                self.stack.pop()
            elif code[self.pl] == 'num':
                self.stack.append(code[self.pl+1])
                self.pl += 1
            elif code[self.pl] == 'str':
                self.stack.append(code[self.pl+1])
                self.pl += 1
            elif code[self.pl] == 'proc':
                self.stack.append(self.pl)
                self.pl += code[self.pl+1]
                self.pl += 1
            elif code[self.pl] == 'call':
                argc = code[self.pl+1]
                args = []
                for _ in range(argc):
                    args.append(self.stack[-1])
                    self.stack.pop()
                args.reverse()
                func = self.stack[-1]
                self.stack.pop()
                self.call(func, *args)
                self.pl += 1
            else:
                print('error')
                exit(1)
            self.pl += 1
        return self.stack[-1]

def parse(code):
    tokens = tokenize(code)
    clumped = clump(tokens)
    lines = split(clumped)
    return lines

def gwlocals():
    return wlocals

wlis = []
def complete(text, state):
    global wlis
    try:
        if state == 0:
            wlis = [i for i in wlocals[0].keys() if i.startswith(text)]
        if state < len(wlis):
            return wlis[state]
        return None
    except:
        print('error')

def repl():
    readline.set_completer(complete)
    readline.parse_and_bind('tab: complete')
    readline.set_completer_delims(' \t\n\r;{}[]()')
    readline.set_history_length(15)
    code = input('PyCL> ')
    lines = parse(code)
    vm = Vm()
    bc = vm.bytecode(lines)
    ret = vm.vmrun(bc)
    if ret != None:
        print(ret)

def main(args):
    if len(args) > 1:
        sw = False
        if args[1].startswith('-e'):
            ec = False
            if args[1] == '-ec':
                ec = True                
            args = args[1:]
            sw = True
        if args[1] == 'perf':
            profile = True
            args = args[1:]
        else:
            profile = False
        code = slurp(args[1])
        lines = parse(code)
        vm = Vm()
        bc = vm.bytecode(lines)
        if sw:
            f = open('out.bytecode', 'w')
            if ec:
                pl = 0
                end = len(bc)
                offset = 0
                offs = []
                while pl < end:
                    if bc[pl] not in ('ret', 'pop'):
                        offs.append(offset)
                        pl += 1
                    offset += 1
                    offs.append(offset)
                    pl += 1
                pl = 0
                opn = 0
                while pl < end:
                    cur = bc[pl]
                    if cur in ('ret', 'pop'):
                        f.write(cur)
                        f.write('\n')
                    elif cur == 'proc':
                        f.write(str(cur))
                        f.write('\n')
                        pl += 1
                        pl1, pl2 = pl, pl+bc[pl]
                        offset = offs[pl2] - offs[pl1] - 1
                        f.write(str(offset))
                        f.write('\n')
                    else:
                        f.write(str(cur))
                        f.write('\n')
                        pl += 1
                        cur = bc[pl]
                        f.write(str(cur))
                        f.write('\n')
                    pl += 1
                    opn += 1
            else:
                for i in bc:
                    f.write(str(i))
                    f.write('\n')
            f.close()
        else:
            if profile:
                import cProfile
                func = lambda: vm.vmrun(bc)
                cProfile.runctx('func()', globals(), locals())
            else:
                vm.vmrun(bc)
    else:
        while True:
            repl()

if __name__ == '__main__':
    main(sys.argv)