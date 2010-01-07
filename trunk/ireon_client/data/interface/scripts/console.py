# -*- coding: utf-8 -*-
class CAnalysator(Cmd):
    output = None
    def __init__(self, sin,sout):
        Cmd.__init__(self,stdin = sin, stdout = sout)
        
    def help_shell(self):
        self.stdout.write('python shell command\n')
            
    def do_shell(self,arg):
        #OutputEx.changeOutputActivity()
        try:
                    exec(arg, globals())
        except (RuntimeError, TypeError, NameError):
                    self.stdout.write('Ошибка во время выполнения\n')
        except SyntaxError:
                    self.stdout.write('Синтаксическая ошибка\n') 
        except AttributeError:
                    self.stdout.write('Не найден атрибут\n') 
    def help_ls(self):
        self.stdout.write('show vars\n')
    def do_ls(self,arg):
        import __main__
        count = 0
        for i in __main__.__dict__:
            
            self.stdout.write(i.rjust(40))
            count+= 1
            if count%4 == 0:
                pass #self.stdout.flush()
        self.stdout.flush()
    def help_authors(self):
        self.stdout.write('Люди, принимавшие участие в работе на текущей итерации\n')
    def do_authors(self,arg):  
        self.stdout.write('Лидер: Зак\n')
        self.stdout.write('Сеттинг: Стрелок, [AD], Captain Quazar, Gildor, Gloom, Koby, Vetrovei, Vilorn, Woolfich, Xstry\n')
        self.stdout.write('Программисты: Зак, LLyeli, Ketzer, mip\n') 
    def help_history(self):
        self.stdout.write('История комманд\n')
        self.stdout.write('use: history [-c]\n')
        self.stdout.write('Выводит историю команд\n')
        self.stdout.write('Аргументы:\n')
        self.stdout.write('c - очистка истории команд\n')
    def do_history(self,arg): 
        pass
        
    def help_position(self):
        self.stdout.write('Текущие координаты и поворот относительно мировых осей вашего персонажа\n')
    def do_position(self, arg):
        if A.state != State.DEAD and A.state != State.PLAY:
            self.stdout.write('Извините, эта команда доступна только в режиме игры\n')
            return
        pos = W.ownChar.position
        self.stdout.write('x = ' + str(pos.x) + ' y = ' + str(pos.y) + ' z = '+ str(pos.z)+'\n')
        self.stdout.write('rotation = ' +  str(W.ownChar.rotation.value) + '\n')  
    

    def help_static(self):
        self.stdout.write('Работа с статичными обьектами в мире\n\n')
        self.stdout.write('use: static - список статичных обьектов, ид и имя прототипа из которого они созданы,\n')
        self.stdout.write('поворот относительно оси Y в радианах, позиция в виде вектора из 3-х компонент\n\n')
        self.stdout.write('use: static <ID> - вывод более подробной информации о статичном обьекте\n')
        self.stdout.write('id обьекта, кватернион, описывающий ориентацию в пространстве, id и имя прототипа,')
        self.stdout.write('поворот относительно оси Y в радианах, позиция в виде вектора из 3-х компонент\n\n')
        self.stdout.write('use: static <ID> < rotr , rotv3r , rotq > для точного задания поворота в радианах, вектора вращения и угла, кватерниона \n\n')
        self.stdout.write('use: static <ID> < movev3, movev2> Для установки позиции предмета.\n\n')
        self.stdout.write('Добавляйте к спецификатору < + > для изменения существующего параметра. Например < rotr+ > \n\n')
        self.stdout.write('use: static <ID> < pitch, roll, yaw > - тангаж, крен, рысканье \n')
        
        #here
    def do_static(self,arg):
        if A.state != State.DEAD and A.state != State.PLAY:
            self.stdout.write('Извините, эта команда доступна только в режиме игры\n')
            return
        param = string.split(arg)
        if len(param) == 0:
            k = 0
            self.stdout.write('ID | protoId | protoName | rotation | position\n')
            for i in W.statics:
                rot = i.rotation
                pos = i.position
                
                self.stdout.write( str(k) +' | ' + str(i.prototype.id) + ' | ' + str(i.prototype.name) + ' | '+ str(rot.value) +' | ' + 'x = ' + str(pos.x) + ' y = ' + str(pos.y) + ' z = '+ str(pos.z)+'\n')
                k+=1
        try:
            static_id = int(param[0])
        except: #here
            pass
            #self.stdout.write('Ошибка \n')
            #return
            
        if len(param) == 1:
            static_orientation_quaternion = W.statics[static_id].orientation 
            self.stdout.write('ID =  ' + str(static_id) + '\n')
            self.stdout.write('Quaternion: x = ' +str(static_orientation_quaternion.x)+ ' y = ' +str(static_orientation_quaternion.y)+  ' z = ' +str(static_orientation_quaternion.z)+ ' w = ' + str(static_orientation_quaternion.z) + '\n')
            rot = W.statics[static_id].rotation
            pos = W.statics[static_id].position
            self.stdout.write('protoID = \'' + str(W.statics[static_id].prototype.id) + '\' protoName = \'' + str(W.statics[static_id].prototype.name) + '\' Rotation = '+ str(rot.value) +' Position = ' + 'x = ' + str(pos.x) + ' y = ' + str(pos.y) + ' z = '+ str(pos.z)+'\n')
        elif len(param) == 2:
            if param[1].startswith('movev3'):
                self.stdout.write('Укажите координаты в виде вектора из 3-х компонент\n')
                return
            elif param[1].startswith('movev2'):
                self.stdout.write('Укажите координаты в виде вектора из 2-х компонент\n')
                return
            elif param[1].startswith('rotr'):
                self.stdout.write('Укажите поворот в радианах < Rad >\n')
                return
            elif param[1].startswith('rotv3r'):
                self.stdout.write('Укажите поворот в виде осей + угол в радианах поворота относительно осей < x y z w >\n')
                return
            elif param[1].startswith('rotq'):
                self.stdout.write('Укажите поворот в виде кватерниона из четырех компонент < x y z w >\n')
                return
            elif param[1].startswith('pitch'):
                self.stdout.write('Укажите поворот в виде радиана\n')
                return
            elif param[1].startswith('roll'):
                self.stdout.write('Укажите поворот в виде радиана\n')
                return
            elif param[1].startswith('yaw'):
                self.stdout.write('Укажите поворот в виде радиана\n')
                return
                         
            else: #here
                self.stdout.write('Неизвестный спецификатор. Используйте < movev3 > или < movev2 > для задания позиции обьекта в трехмерном или двухмерном пространстве \n')
                self.stdout.write('< rotr >, < rotv3r >, < rotq > для точного задания поворота в радианах, вектора вращения и угла, кватерниона \n')
                self.stdout.write('Добавляйте к спецификатору < + > для изменения существующего параметра \n')
                self.stdout.write('Используйте pitch ( тангаж ), roll ( крен ), yaw ( рысканье )')
        elif len(param) >= 3:
            if param[1].startswith('rotr'):
                rotation = float(param[2])   
                static_rotation_radian = Radian(rotation)
                if param[1].endswith('+'):
                    old_static_rotation_radian = W.statics[static_id].rotation
                    W.statics[static_id].rotation = old_static_rotation_radian + static_rotation_radian 
                else:
                    W.statics[static_id].rotation = static_rotation_radian 
                self.stdout.write('Rotation = ' + str(W.statics[static_id].rotation.value) + '\n')
                orientation = W.statics[static_id].orientation
                self.stdout.write('Quaternion: x = ' +str(orientation.x)+ ' y = ' +str(orientation.y)+  ' z = ' +str(orientation.z)+ ' w = ' + str(orientation.w) + '\n')
            
            elif param[1].startswith('rotv3r') :
                if len(param) == 5:
                    axis = Vector3(float(param[2]), float(param[3]), float(param[4]))
                    rot = Radian(3.14) # Pi
                elif len(param) >= 6:
                    axis = Vector3(float(param[2]), float(param[3]), float(param[4])) 
                    rot = Radian(float(param[5]))
                else:
                    self.stdout.write('Вектор осей должен состоять из трех компонент \n')
                    return
                static_orientation_quaternion = Quaternion(rot, axis)
                if param[1].endswith('+'):
                    W.statics[static_id].rotateV3R(axis, rot)
                else:
                    W.statics[static_id].orientation = static_orientation_quaternion
                orientation = W.statics[static_id].orientation   
                self.stdout.write('Quaternion: x = ' +str(orientation.x)+ ' y = ' +str(orientation.y)+  ' z = ' +str(orientation.z)+ ' w = ' + str(orientation.z) + '\n')
                
            elif param[1].startswith('rotq'):
                if len(param) >= 6:
                    # x,y,z,w (2<->5 constructor parameters)
                    static_orientation_quaternion = Quaternion(float(param[5]), float(param[3]), float(param[4]), float(param[2]) )
                else:
                    self.stdout.write('Кватернион должен состоять из четырех компонент \n')
                    return
                
                if param[1].endswith('+'):
                    W.statics[static_id].rotateQ(static_orientation_quaternion)
                else:
                    W.statics[static_id].orientation = static_orientation_quaternion
                orientation = W.statics[static_id].orientation     
                self.stdout.write('Quaternion: x = ' +str(orientation.x)+ ' y = ' +str(orientation.y)+  ' z = ' +str(orientation.z)+ ' w = ' + str(orientation.w) + '\n')
            elif param[1].startswith('pitch'):
                if len(param) >= 3:
                    rot = Radian(float(param[2]))
                else:
                    self.stdout.write('Требуется указать угол в виде радиана\n')
                    return
                W.statics[static_id].pitch(rot)
                 
            elif param[1].startswith('roll'):
                if len(param) >= 3:
                    rot = Radian(float(param[2]))
                else:
                    self.stdout.write('Требуется указать угол в виде радиана\n')
                    return
                W.statics[static_id].roll(rot)  
                
            elif param[1].startswith('yaw'):
                if len(param) >= 3:
                    rot = Radian(float(param[2]))
                else:
                    self.stdout.write('Требуется указать угол в виде радиана\n')
                    return 
                W.statics[static_id].yaw(rot)  
            
            elif param[1].startswith('movev2'):
                if len(param) >= 4:
                    pos = Vector2(float(param[2]), float(param[3])) 
                    
                else:
                    self.stdout.write('Требуется указать вектор из 2-х компонент\n')
                    return
                if param[1].endswith('+'):
                    old_pos = W.statics[static_id].position2 
                    W.statics[static_id].position2 = pos + old_pos  
                else:
                    W.statics[static_id].position2 = pos  
                
                
            elif param[1].startswith('movev3'):
                if len(param) >= 5:
                    pos = Vector3(float(param[2]), float(param[3]), float(param[4])) 
                else:
                    self.stdout.write('Требуется указать вектор из 3-х компонент\n')
                    return 
                if param[1].endswith('+'):
                    old_pos = W.statics[static_id].position 
                    W.statics[static_id].position = pos + old_pos  
                else:
                    W.statics[static_id].position = pos   
           
################################################################################

class CConsole(GenericComposite):
    m_messages = ''
    m_active = True
    analysator = None
    history = None
    def create(self, name = ''):
        self.analysator = CAnalysator(sin = None, sout = self)
        self.history = CHistory(30)
        self['ConsoleWindow'] = win2composite(GW.getWindow('ConsoleWindow')) 
        
        self['ConsoleWindow']['MainWindow'] = win2leaf(GW.getWindow('ConsoleWindow/MainWindow'))                 
        self['ConsoleWindow']['Channel'] = win2leaf(GW.getWindow('ConsoleWindow/Channel'))  
        self['ConsoleWindow']['Edit'] = win2leaf(GW.getWindow('ConsoleWindow/Edit'))                           
        
        handler = exportToMainDict(self.consoleEditKeyHandler)
        
        self['ConsoleWindow']['Edit'].content.subscribeScriptedEvent("KeyDown",handler)

        self['ConsoleWindow'].content.addChildWindow(self['ConsoleWindow']['MainWindow'].content)
        self['ConsoleWindow'].content.addChildWindow( self['ConsoleWindow']['Edit'].content)
        
        OutputEx.reg(self.write)
        self['Scroll'] = win2leaf(GW.getWindow(self['ConsoleWindow']['Channel'].content.Name + '__auto_vscrollbar__'))
                
        
        self.write('console started....\n')
        self['ConsoleWindow']['Channel'].content.Text = self.m_messages
        self['ConsoleWindow']['Channel'].content.AlwaysOnTop = True
        self['ConsoleWindow']['Edit'].content.AlwaysOnTop = True
        
        self.content = self['ConsoleWindow'].content
                   
        return self
           
    def consoleEditKeyHandler(self, arg):
        if arg.Scancode == Key.Return:
            text = self['ConsoleWindow']['Edit'].content.Text
            if text == '':
                pass
            else:
                self.write(">>> "+text+"\n")
                self.analysator.onecmd(text)
                self['ConsoleWindow']['Channel'].content.Text = self.m_messages
                #self['Scroll'].content.toEnd()
                self['Scroll'].content.ScrollPosition = self['Scroll'].content.DocumentSize - self['Scroll'].content.PageSize
                self['ConsoleWindow']['Edit'].content.text = ''
                self.history.add(text)
        if arg.Scancode == Key.Tab:
            text = self['ConsoleWindow']['Edit'].content.Text
        if arg.Scancode == Key.PageUp:
                self['Scroll'].content.Position -= self['Scroll'].content.getPageSize() 
        if arg.Scancode == Key.PageDown:
                self['Scroll'].content.Position += self['Scroll'].content.getPageSize() 
        if arg.Scancode == Key.ArrowUp:
                self['ConsoleWindow']['Edit'].content.Text =  self.history.get()
                self.history.back()
        if arg.Scancode == Key.ArrowDown:
                self.history.forward()
                self['ConsoleWindow']['Edit'].content.Text =  self.history.get() 
    
    def write(self,msg):
        self.m_messages += msg 
    def flush(self):
        self.m_messages += '\n'
################################################################################

class CHistory:
    buffer = []
    sizeHistory = 1
    indexHistory = None
    def __init__(self, sizeHistory):
        self.sizeHistory = sizeHistory
    def add(self, command):
        try:
            self.buffer.remove(command)
        except ValueError:
            pass
        if len(self.buffer) >= self.sizeHistory:
            del self.buffer[0]
        self.buffer.append(command)
        self.indexHistory = self.buffer.index(command)
    
    def clear():
        self.buffer.clear()
        self.indexHistory = None
    def back(self):
        if self.indexHistory == None:
            return
        elif self.indexHistory > 0 :
            self.indexHistory -= 1
        
    def forward(self):
        if self.indexHistory == None:
            return
        elif self.indexHistory < len(self.buffer) - 1:
            self.indexHistory += 1
        
    def get(self):
        if self.indexHistory != None and len(self.buffer) != 0:
            return self.buffer[self.indexHistory]
        else:
            return ''