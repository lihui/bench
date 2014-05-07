package main

import (
	"fmt"
	"io/ioutil"
	"time"
)

type CsvParser struct {
	data []byte
	pos  int
}

const newLine = '\n'
const quote = '"'
const comm = ','
const zero = '\u0000'

func New(data []byte) *CsvParser {
	return &CsvParser{append(trim(data), zero), 0}
}
func (this *CsvParser) parseCsv() [][][]byte {
	var rs [][][]byte
	for {
		rs = append(rs, this.parseLine())
		if !this.consume(newLine) {
			break
		}

	}
	return rs
}

func (this *CsvParser) parseLine() [][]byte {
	var row [][]byte
	for {
		row = append(row, trim(this.parseField()))
		if !this.consume(comm) {
			break
		}
	}
	return row
}

func (this *CsvParser) parseField() []byte {
	if this.next(quote) {
		return this.parseQField()
	}
	return this.parseSField()
}
func (this *CsvParser) parseSField() []byte {
	start := this.pos
	for this.data[this.pos] != zero && this.data[this.pos] != newLine && this.data[this.pos] != comm {
		this.pos++
	}
	return this.data[start:this.pos]
}
func (this *CsvParser) parseQField() []byte {
	this.consume(quote)
	start := this.pos
	for this.data[this.pos] != zero {
		if this.data[this.pos] == quote && this.data[this.pos+1] == quote {
			this.pos += 2
			continue
		}
		if this.data[this.pos] == quote {
			break
		}
		this.pos++
	}
	this.consume(quote)
	return this.data[start:this.pos]
}

func isWhite(ch byte) bool {
	return ch == ' ' || ch == '\n' || ch == '\r' || ch == 't'
}
func trim(bytes []byte) []byte {
	start := 0
	end := len(bytes) - 1
	if start > end {
		return bytes
	}
	for isWhite(bytes[start]) {
		start++
	}
	for isWhite(bytes[end]) {
		end--
	}
	return bytes[start : end+1]
}

func (this *CsvParser) next(ch byte) bool {
	p := this.pos
	for isWhite(this.data[this.pos]) {
		if this.data[p] == ch {
			break
		}
		p++
	}
	return this.data[p] == ch
}

func (this *CsvParser) consume(ch byte) bool {

	p := this.pos
	for isWhite(this.data[p]) {
		if this.data[p] == ch {
			break
		}
		p++
	}
	if this.data[p] == ch {
		this.pos = p + 1
		return true
	}
	return false
}

func parse(data []byte) [][][]byte {
	var p = New(data)
	return p.parseCsv()
}
func checkErr(e error) {
	if e != nil {
		panic(e)
	}
}
func main() {
	content, err := ioutil.ReadFile("test.csv")
	checkErr(err)

	var r [][][]byte

	s := time.Now()
	for i := 0; i < 1000; i++ {
		r = parse(content)
	}
	t := time.Since(s)
	for _, row := range r {
		for _, field := range row {
			fmt.Print(string(field) + ",")
		}
		fmt.Printf("\n")
	}
	fmt.Printf("%v\n", t)
	fmt.Printf("size=%v\n", len(r))

}

