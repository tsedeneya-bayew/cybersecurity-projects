function PLC_EDR_3D(model)
% Live visualization of the existing Simulink/CODESYS water-tank lab.
% Run with the model STOPPED: PLC_EDR_3D('PLC_EDR_WaterTank_3D')
% Uses base MATLAB graphics; no fake signal data or separate Modbus client.
if nargin == 0, model = bdroot; end
model = char(model);
load_system(model);
assert(strcmp(get_param(model,'SimulationStatus'),'stopped'), ...
    'Stop Simulink before opening the 3D view.');
assert(endsWith(model,'_3D'), ...
    'Save a model copy ending in _3D first, then pass its name.');
levelBlock = [model '/Integrator'];
errorBlock = [model '/Abs'];
displayBlock = [model '/Display'];
gains = find_system(model,'SearchDepth',1,'BlockType','Gain');
values = cellfun(@(b) str2double(get_param(b,'Gain')),gains);
reportCandidates = gains(abs(values-0.01)<1e-9);
pumpCandidates = gains(abs(values-5)<1e-9);
assert(numel(reportCandidates)==1 && numel(pumpCandidates)==1, ...
    'Expected exactly one Gain 0.01 and one Gain 5. Send the gain settings for help.');
reportBlock = reportCandidates{1};
pumpBlock = pumpCandidates{1};
get_param(levelBlock,'Handle'); get_param(errorBlock,'Handle');
get_param(displayBlock,'Handle');
% Keep runtime signals readable in Normal mode. Changes affect only the copy.
set_param(model,'SimulationMode','normal','BlockReduction','off', ...
    'OptimizeBlockIOStorage','off','EnablePacing','on','PacingRate','1');
old = findall(groot,'Type','figure','Tag','PLC_EDR_3D');
delete(old);
fig = figure('Name','Live PLC / Sensor Integrity — 3D Tank', ...
    'NumberTitle','off','Tag','PLC_EDR_3D','Color',[0.06 0.09 0.14], ...
    'Position',[100 100 1100 680]);
ax = axes(fig,'Position',[0.04 0.14 0.59 0.8], ...
    'Color',[0.06 0.09 0.14],'XColor','w','YColor','w','ZColor','w');
hold(ax,'on'); view(ax,[-38 24]); axis(ax,'equal');
xlim(ax,[-2.3 2]); ylim(ax,[-1.6 1.6]); zlim(ax,[0 3.2]);
grid(ax,'on'); zlabel(ax,'Tank height');
title(ax,'LIVE SIMULINK WATER TANK','Color','w');
theta = linspace(0,2*pi,64); radius = 0.9; height = 2.8;
[X,Y,Z] = cylinder(radius,63);
surf(ax,X,Y,Z*height,'FaceColor',[0.7 0.85 0.95], ...
    'FaceAlpha',0.10,'EdgeColor','none');
plot3(ax,radius*cos(theta),radius*sin(theta),zeros(size(theta)),'Color','w');
plot3(ax,radius*cos(theta),radius*sin(theta),height*ones(size(theta)),'Color','w');
water = surf(ax,X,Y,Z*0.001,'FaceColor',[0.04 0.55 0.95], ...
    'FaceAlpha',0.65,'EdgeColor','none');
surface = patch(ax,radius*cos(theta),radius*sin(theta),zeros(size(theta)), ...
    [0.05 0.75 1],'FaceAlpha',0.85,'EdgeColor','none');
reportRing = plot3(ax,0.94*cos(theta),0.94*sin(theta),zeros(size(theta)), ...
    '--','Color',[1 0.72 0.12],'LineWidth',2);
pipe = plot3(ax,[-1.9 -1.3 -1.3 -0.9],[0 0 0 0], ...
    [0.3 0.3 2.6 2.6],'Color',[0.4 0.45 0.5],'LineWidth',12);
text(ax,-2,0,0.05,'PUMP','Color','w');
text(ax,0,-1.2,3,'Blue: plant   Gold: PLC report','Color','w');
panel = uicontrol(fig,'Style','text','Units','normalized', ...
    'Position',[0.65 0.29 0.32 0.60],'BackgroundColor',[0.10 0.14 0.20], ...
    'ForegroundColor','w','FontSize',15,'HorizontalAlignment','left', ...
    'String',sprintf('WAITING FOR SIMULINK\n\nPress Run in your model.'));
uicontrol(fig,'Style','text','Units','normalized','Position',[0.04 0.02 0.93 0.09], ...
    'BackgroundColor',fig.Color,'ForegroundColor',[0.75 0.8 0.85], ...
    'FontSize',10,'String',['Live model signals only | Rotate tank with mouse | ' ...
    'Toggle AttackEnable in CODESYS with Debug > Write Values']);
rotate3d(fig,'on');
lastError = '';
tm = timer('ExecutionMode','fixedSpacing','Period',0.15, ...
    'BusyMode','drop','TimerFcn',@refresh);
fig.CloseRequestFcn = @closeView;
start(tm);
fprintf('3D view ready for %s. Start CODESYS, then click Simulink Run.\n',model);

    function refresh(~,~)
        if ~isgraphics(fig), stop(tm); delete(tm); return; end
        try
            status = get_param(model,'SimulationStatus');
            if ~any(strcmp(status,{'running','paused'}))
                panel.String = sprintf('MODEL %s\n\nPress Run in Simulink.\nLast tank image is frozen.',upper(status));
                panel.ForegroundColor = [0.8 0.85 0.9];
                drawnow limitrate; return;
            end
            actual = sample(levelBlock,'out');
            reported = sample(reportBlock,'out');
            mismatch = sample(errorBlock,'out');
            pump = sample(pumpBlock,'in');
            alert = sample(displayBlock,'in');
            time = get_param(model,'SimulationTime');
            h = height*max(0,min(100,actual))/100;
            water.ZData = Z*h;
            surface.ZData = h*ones(size(theta));
            reportRing.ZData = height*max(0,min(100,reported))/100*ones(size(theta));
            if pump~=0, pipe.Color = [0.15 0.9 0.5]; pumpText='ON';
            else, pipe.Color = [0.4 0.45 0.5]; pumpText='OFF'; end
            if alert~=0, state='MISMATCH ALERT'; panel.ForegroundColor=[1 0.3 0.3];
            else, state='WITHIN THRESHOLD'; panel.ForegroundColor=[0.3 1 0.65]; end
            panel.String = sprintf(['%s\n\nSimulation: %.1f s\n\nPlant level: %.2f %%' ...
                '\n\nPLC report: %.2f %%\n\nDifference: %.2f pp' ...
                '\n\nPump: %s\n\nAlert: %d\n\nThreshold: >2 pp'], ...
                state,time,actual,reported,mismatch,pumpText,alert~=0);
            lastError=''; drawnow limitrate;
        catch err
            if isgraphics(panel)
                panel.ForegroundColor = [1 0.7 0.2];
                panel.String = sprintf('LIVE DATA UNAVAILABLE\n\n%s',err.message);
            end
            if ~strcmp(lastError,err.message)
                warning('PLC_EDR_3D:ReadFailure','%s',err.message);
                lastError=err.message;
            end
        end
    end
    function value = sample(block,direction)
        rto = get_param(block,'RuntimeObject');
        assert(~isempty(rto),'Runtime signal unavailable: %s',block);
        if strcmp(direction,'out'), value=double(rto.OutputPort(1).Data);
        else, value=double(rto.InputPort(1).Data); end
        assert(isscalar(value) && isfinite(value),'Invalid live signal: %s',block);
    end
    function closeView(~,~)
        if isvalid(tm), stop(tm); delete(tm); end
        delete(fig);
    end
end
