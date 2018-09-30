const fs = require("fs");
const ssh2 = require("ssh2");
const child_pty = require("child_pty");
const { exec } = require("child_process");

const minColumns = 103;
const minRows = 24;

const HOST_KEY_FILE = "settings/ssh_host_rsa_key";

if (!fs.existsSync(HOST_KEY_FILE)) {
    console.log("Generating host key");
    exec("ssh-keygen -f " + HOST_KEY_FILE + " -N '' -t rsa", (err, stdout, stderr) => {
        if (err) {
            console.error("Error generating host key");
            console.error(stderr);
        } else {
            console.log("Generated host key");
        }
        init();
    });
} else {
    init();
}

function init() {
    new ssh2.Server({
        hostKeys: [ fs.readFileSync(HOST_KEY_FILE) ],
    }, function (client, info) {
        console.log("New connection", info);
        let columns = 103;
        let rows = 24;
        let ls;

        let stream;
        client.on("authentication", function (ctx) {
            return ctx.accept();
        }).on("ready", function () {
            client.once("session", function (accept, reject) {
                accept().once("pty", function (accept, reject, info) {
                    setSize(info.cols, info.rows);
                    accept && accept();

                }).on("window-change", function (accept, reject, info) {
                    setSize(info.cols, info.rows);

                    if (stream) {
                        stream.rows = rows;
                        stream.columns = columns;
                        stream.emit("resize");
                    }
                    if (accept)
                        accept();

                    if (ls) {
                        ls.stdout.resize({ columns: columns + 1, rows });
                        ls.stdout.resize({ columns, rows });
                    }
                }).once("shell", function (accept, reject) {
                    stream = accept();
                    ls = child_pty.spawn("./pong", []);
                    ls.stdout.resize({ columns, rows });

                    ls.stdout.pipe(stream);
                    stream.pipe(ls.stdin);
                });
            });
        }).on("end", function () {
            quitPong(ls);
            console.log("Disconnected", info);
        }).on("error", function (err) {
            quitPong(ls);
            console.error(err);
        });

        function quitPong(child) {
            if (!child) {
                return;
            }
            child.kill();
            setTimeout(() => {
                if (child.killed) {
                    child.kill("SIGKILL");
                }
            }, 1000);
        }

        function setSize(newColumns, newRows) {
            rows = newRows;
            columns = newColumns;
            if (rows < minRows)
                rows = minRows;
            if (columns < minColumns)
                columns = minColumns;
        }

    }).listen(22, function () {
        console.log("Listening on port " + this.address().port);
    });

}
