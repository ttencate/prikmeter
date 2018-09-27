function startup()
    if file.open('init.lua') == nil then
        print('init.lua deleted or renamed, aborting startup')
    else
        file.close('init.lua')
        print('Launching main program')
        dofile('main.lua')
    end
end

print('Startup will resume in 3 seconds. To abort, run file.remove("init.lua") on the serial console.')
tmr.create():alarm(3000, tmr.ALARM_SINGLE, startup)
