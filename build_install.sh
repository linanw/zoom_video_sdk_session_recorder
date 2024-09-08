sudo apt update
sudo apt -y install libcurl4-openssl-dev
cmake -B build
cd build
make
cd ..

set -x

sudo cp claw_machine_client.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable claw_machine_client.service
sudo bash -c "echo -e '\nKERNEL==\"video0\", SYMLINK=\"video0\", TAG+=\"systemd\"\nKERNEL==\"video1\", SYMLINK=\"video1\", TAG+=\"systemd\"' >> /lib/udev/rules.d/99-systemd.rules"
