# this file will create the config file in ~/.ledit
# and also copy the themes over

mkdir ~/.ledit
mkdir ~/.ledit/themes
cp -r themes/* ~/.ledit/themes

echo "diff_enabled:1" >> ~/.ledit/main.config
echo "line_enabled:1" >> ~/.ledit/main.config
echo "sess_enabled:1" >> ~/.ledit/main.config
echo "theme:default" >> ~/.ledit/main.config

