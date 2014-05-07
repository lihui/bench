
import "dart:io";

class CsvParser {
    static final String newLine = '\n';
    static final String quote = '"';
    static final String comm = ',';
    static final String zero = '\u0000';
    String _data = "";
    int _pos = 0;

    CsvParser(String data) {
        _data = data.trim()+newLine+zero;
    }
    static List<List<String>> parse(String data) {
        var parser = new CsvParser(data);
        return parser.parseCsv();
    }
    List<List<String>> parseCsv() {
        var rs = new List<List<String>>();
        do {
            rs.add(_parseLine());
            consume(newLine);
        } while (!next(zero));
        return rs;
    }
    List<String> _parseLine() {
        var row = new List<String>();
        do {
            row.add(_parseField().trim());
        } while (consume(comm));
        return row;
    }
    String _parseField() {
        if (next(quote)) return _parseQField();
        return _parseSField();
    }

    String _parseSField() {
        var start = _pos;
        while (_data[_pos] != newLine && _data[_pos] !=
                comm) {
            _pos++;
        }
        return _data.substring(start, _pos);

    }
    String _parseQField() {
        consume(quote);
        var start = _pos;
        while (true) {
            if (_data[_pos] == quote && _data[_pos + 1] == quote) {
                _pos += 2;
                continue;
            }
            if (_data[_pos] == quote) break;
            _pos++;
        }
        consume(quote);
        return _data.substring(start, _pos - 1);

    }

    static bool isWhite(String ch) {
        return ' ' == ch || '\n' == ch || '\r' == ch || '\t' == ch;
    }
    bool next(String ch) {
        var p = _pos;
        while (isWhite(_data[p])) {
            if (_data[p] == ch) break;
            p++;
        }
        return _data[p] == ch;

    }

    bool consume(String ch) {
        var p = _pos;
        while (isWhite(_data[p])) {
            if (_data[p] == ch) break;
            p++;
        }
        if (_data[p] == ch) {
            _pos = p + 1;
            return true;
        }
        return false;
    }
}

main() {
    var file = new File(r"test.csv");
    var content = file.readAsStringSync();
    var st = new Stopwatch()..start();
    var r = [];
    for (int i = 0; i < 1000; i++) r = CsvParser.parse(content);
    st.stop();
    for (var row in r) {
        print(row);
    }
    print("${st.elapsedMilliseconds} ms");
    print("size=${r.length}");

}


