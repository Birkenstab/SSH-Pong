const fs = require('fs');
const ssh2 = require('ssh2');
const child_pty = require("child_pty");

const minColumns = 103;
const minRows = 24;

new ssh2.Server({
    hostKeys: [fs.readFileSync('host.key')],
}, function (client) {
    let columns = 103;
    let rows = 24;
    let ls;

    let stream;
    client.on('authentication', function (ctx) {
        return ctx.accept();
    }).on('ready', function () {
        client.once('session', function (accept, reject) {
            accept().once('pty', function (accept, reject, info) {
                setSize(info.cols, info.rows);
                accept && accept();

            }).on('window-change', function (accept, reject, info) {
                setSize(info.cols, info.rows);

                if (stream) {
                    stream.rows = rows;
                    stream.columns = columns;
                    stream.emit('resize');
                }
                if (accept)
                    accept();

                ls.stdout.resize({columns: columns + 1, rows});
                ls.stdout.resize({columns, rows});
            }).once('shell', function (accept, reject) {
                stream = accept();

                ls = child_pty.spawn('./pong', []);
                ls.stdout.resize({columns, rows});

                ls.stdout.pipe(stream);
                stream.pipe(ls.stdin);
            });
        });
    }).on('end', function () {
        console.log("Disconnected");
    }).on('error', function (err) {
        console.error(err);
    });

    function setSize(newColumns, newRows) {
        rows = newRows;
        columns = newColumns;
        if (rows < minRows)
            rows = minRows;
        if (columns < minColumns)
            columns = minColumns;
    }
}).listen(10022, function () {
    console.log('Listening on port ' + this.address().port);
});
