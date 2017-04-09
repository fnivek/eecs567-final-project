function varargout = ArmGUI(varargin)
%ARMGUI MATLAB code file for ArmGUI.fig
%      ARMGUI, by itself, creates a new ARMGUI or raises the existing
%      singleton*.
%
%      H = ARMGUI returns the handle to a new ARMGUI or the handle to
%      the existing singleton*.
%
%      ARMGUI('Property','Value',...) creates a new ARMGUI using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to ArmGUI_OpeningFcn.  This calling syntax produces a
%      warning when there is an existing singleton*.
%
%      ARMGUI('CALLBACK') and ARMGUI('CALLBACK',hObject,...) call the
%      local function named CALLBACK in ARMGUI.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help ArmGUI

% Last Modified by GUIDE v2.5 03-Apr-2017 20:50:54

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ArmGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @ArmGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
   gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


function ArmGUI_TimerFcn(obj, event, link, guiHandle)
% Get most recent handles
handles = guidata(guiHandle);

% Check connection status
stat = get(link.portStream, 'Status');

if strcmp(stat, 'closed')
    
elseif strcmp(stat, 'open')
    nAvailable = get(link.portStream, 'BytesAvailable');
    
    if nAvailable > 0
        r = fread(link.portStream, nAvailable);
        fprintf('Read in %d\n', nAvailable);
        starts = find(r == hex2dec('AA'));
        for q = 1:length(starts)
            if q == length(starts)
                msg = r(starts(q):end);
            else
                msg = r(starts(q):(starts(q + 1) - 1));
            end
            link.parseMessage(msg);
        end
    end
    
    % Set labels
    set(handles.editPosX,'String',sprintf('%0.4f',link.endEffectorPos(1)));
    set(handles.editPosY,'String',sprintf('%0.4f',link.endEffectorPos(2)));
    set(handles.editPosZ,'String',sprintf('%0.4f',link.endEffectorPos(3)));
    
    set(handles.editQ1,'String',sprintf('%0.4f',link.jointAngles(1)));
    set(handles.editQ2,'String',sprintf('%0.4f',link.jointAngles(2)));
    set(handles.editQ3,'String',sprintf('%0.4f',link.jointAngles(3)));
    set(handles.editQ4,'String',sprintf('%0.4f',link.jointAngles(4)));
end

% Redraw
PlotArm(handles.axesVisualizer, link.jointAngles);
drawnow;


% --- Executes just before ArmGUI is made visible.
function ArmGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for ArmGUI
handles.output = hObject;

% Create ArmLink object for accessing USB data
link = ArmLink();
handles.armLink = link;

% Timer object for grabbing serial data and updating windows
armTimer = timer('TimerFcn', {@ArmGUI_TimerFcn, link, hObject},...
    'Period', 1/120,...
    'ExecutionMode', 'FixedSpacing',...
    'StartDelay', 1.0);
start(armTimer);
handles.armTimer = armTimer;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes ArmGUI wait for user response (see UIRESUME)
% uiwait(handles.armGUIfigure);


% --- Outputs from this function are returned to the command line.
function varargout = ArmGUI_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function editPortname_Callback(hObject, eventdata, handles)
% hObject    handle to editPortname (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editPortname as text
%        str2double(get(hObject,'String')) returns contents of editPortname as a double


% --- Executes during object creation, after setting all properties.
function editPortname_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editPortname (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in toggleSerialConnect.
function toggleSerialConnect_Callback(hObject, eventdata, handles)
% hObject    handle to toggleSerialConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of toggleSerialConnect
pressState = get(hObject,'Value');

if pressState
    % User pressed down, try to connect to COM port
    
    port = get(handles.editPortname,'String');
    handles.armLink.portName = port;
    
    try
        handles.armLink.open();
        set(handles.textCOMStatus,'String','Connected');
        set(handles.textCOMStatus,'ForegroundColor',[0.0 0.8 0.0]);
        set(handles.textCOMStatus,'Visible','on');
        set(handles.editPortname,'Enable','inactive');
        set(hObject,'String','Disconnect');
    catch e
        set(hObject,'Value',0);
        set(handles.textCOMStatus,'String','Error!');
        set(handles.textCOMStatus,'ForegroundColor',[1.0 0.0 0.0]);
        set(handles.textCOMStatus,'Visible','on');
    end
else
    handles.armLink.close();
    set(handles.textCOMStatus,'String','Disconnected');
    set(handles.textCOMStatus,'ForegroundColor',[0.0 0.0 0.0]);
    set(handles.editPortname,'Enable','on');
    set(hObject,'String','Connect');
end



function editQ1_Callback(hObject, eventdata, handles)
% hObject    handle to editQ1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editQ1 as text
%        str2double(get(hObject,'String')) returns contents of editQ1 as a double


% --- Executes during object creation, after setting all properties.
function editQ1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editQ1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editQ3_Callback(hObject, eventdata, handles)
% hObject    handle to editQ3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editQ3 as text
%        str2double(get(hObject,'String')) returns contents of editQ3 as a double


% --- Executes during object creation, after setting all properties.
function editQ3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editQ3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editQ2_Callback(hObject, eventdata, handles)
% hObject    handle to editQ2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editQ2 as text
%        str2double(get(hObject,'String')) returns contents of editQ2 as a double


% --- Executes during object creation, after setting all properties.
function editQ2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editQ2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editQ4_Callback(hObject, eventdata, handles)
% hObject    handle to editQ4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editQ4 as text
%        str2double(get(hObject,'String')) returns contents of editQ4 as a double


% --- Executes during object creation, after setting all properties.
function editQ4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editQ4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editPosX_Callback(hObject, eventdata, handles)
% hObject    handle to editPosX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editPosX as text
%        str2double(get(hObject,'String')) returns contents of editPosX as a double


% --- Executes during object creation, after setting all properties.
function editPosX_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editPosX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editPosZ_Callback(hObject, eventdata, handles)
% hObject    handle to editPosZ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editPosZ as text
%        str2double(get(hObject,'String')) returns contents of editPosZ as a double


% --- Executes during object creation, after setting all properties.
function editPosZ_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editPosZ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editPosY_Callback(hObject, eventdata, handles)
% hObject    handle to editPosY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editPosY as text
%        str2double(get(hObject,'String')) returns contents of editPosY as a double


% --- Executes during object creation, after setting all properties.
function editPosY_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editPosY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object deletion, before destroying properties.
function axesVisualizer_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to axesVisualizer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function axesVisualizer_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axesVisualizer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axesVisualizer


% --- Executes during object creation, after setting all properties.
function armGUIfigure_CreateFcn(hObject, eventdata, handles)
% hObject    handle to armGUIfigure (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes when user attempts to close armGUIfigure.
function armGUIfigure_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to armGUIfigure (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Stop the timer
if isfield(handles, 'armTimer')
    stop(handles.armTimer);
end

% Close the port if it's open
if isfield(handles, 'armLink')
    if strcmp(get(handles.armLink.portStream,'Status'), 'open')
        handles.armLink.close();
    end
end

% Hint: delete(hObject) closes the figure
delete(hObject);
