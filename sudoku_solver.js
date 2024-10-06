class Square {
  constructor(startRow, endRow, startCol, endCol) {
    this.startRow = startRow;
    this.endRow = endRow;
    this.startCol = startCol;
    this.endCol = endCol;
  }
}

class TwoOccurrences {
  constructor(row1, col1, row2, col2, number) {
    this.row1 = row1;
    this.col1 = col1;
    this.row2 = row2;
    this.col2 = col2;
    this.number = number;
  }
}

class Cell {
  constructor() {
    this.square = 0;
    this.row = 0;
    this.col = 0;
    this.numPossibilities = 0;
    this.possibles = Array(9).fill(false); // Initialize all possibilities to false
    this.certainty = 0; // 0 means uncertain
  }

  countPossibilities() {
    this.numPossibilities = this.possibles.filter(Boolean).length;
  }
}

// Check if a number is present in a specified row or column
function hasNumber(number, isRow, rowOrColIndex, table) {
  for (let i = 0; i < 9; i++) {
    if (isRow) {
      if (table[rowOrColIndex][i].certainty === number) return true;
    } else {
      if (table[i][rowOrColIndex].certainty === number) return true;
    }
  }
  return false;
}

// Check if a number exists in a specific square
function hasNumberInSquare(number, squareIndex, table, squareList) {
  for (
    let i = squareList[squareIndex].startRow;
    i < squareList[squareIndex].endRow;
    i++
  ) {
    for (
      let j = squareList[squareIndex].startCol;
      j < squareList[squareIndex].endCol;
      j++
    ) {
      if (table[i][j].certainty === number) return true;
    }
  }
  return false;
}

// Update the table with a certain number in a cell
function updateTable(number, row, col, table, squares, squareList) {
  // Set all possibilities for this cell to false and mark the number as certain
  table[row][col].possibles.fill(false);
  table[row][col].possibles[number - 1] = true;
  table[row][col].numPossibilities = 1;

  // Remove possibilities from the corresponding row
  for (let j = 0; j < 9; j++) {
    table[row][j].possibles[number - 1] = false;
    table[row][j].countPossibilities();
  }

  // Remove possibilities from the corresponding column
  for (let i = 0; i < 9; i++) {
    table[i][col].possibles[number - 1] = false;
    table[i][col].countPossibilities();
  }

  // Remove possibilities from the corresponding square
  const squareIndex = squares[row][col];
  for (
    let i = squareList[squareIndex].startRow;
    i < squareList[squareIndex].endRow;
    i++
  ) {
    for (
      let j = squareList[squareIndex].startCol;
      j < squareList[squareIndex].endCol;
      j++
    ) {
      if (i !== row || j !== col) {
        table[i][j].possibles[number - 1] = false;
        table[i][j].countPossibilities();
      }
    }
  }
}

function deepCopyTable(table1, table2) {
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      table1[i][j] = new Cell();
      table1[i][j].certainty = table2[i][j].certainty;
      table1[i][j].row = table2[i][j].row;
      table1[i][j].col = table2[i][j].col;
      table1[i][j].square = table2[i][j].square;
      table1[i][j].numPossibilities = table2[i][j].numPossibilities;
      for (let k = 0; k < 9; k++) {
        table1[i][j].possibles[k] = table2[i][j].possibles[k];
      }
    }
  }
}

// Main function to resolve the Sudoku puzzle
function resolve(table, squareList, squares) {
  console.log("start resolve");
  let changed = true;
  let possiblePairs = [];

  while (changed) {
    changed = false;

    // Step 2: Check for cells with only one possibility
    for (let i = 0; i < 9; i++) {
      for (let j = 0; j < 9; j++) {
        if (table[i][j].numPossibilities === 1 && table[i][j].certainty === 0) {
          for (let k = 0; k < 9; k++) {
            if (table[i][j].possibles[k]) {
              table[i][j].certainty = k + 1;
              updateTable(k + 1, i, j, table, squares, squareList);
              console.log(`Certain by cell: ${i} ${j} num:${k + 1}`);
              changed = true;
              break;
            }
          }
        }
      }
    }

    // Check each row for unique numbers
    for (let i = 0; i < 9; i++) {
      for (let k = 0; k < 9; k++) {
        let count = 0;
        let colIndex;
        for (let j = 0; j < 9; j++) {
          if (table[i][j].possibles[k]) {
            count++;
            colIndex = j;
          }
        }
        if (count === 1 && table[i][colIndex].certainty === 0) {
          table[i][colIndex].certainty = k + 1;
          updateTable(k + 1, i, colIndex, table, squares, squareList);
          console.log(`Certain by row: ${i} ${colIndex} num:${k + 1}`);
          changed = true;
        }
      }
    }

    // Check each column for unique numbers
    for (let j = 0; j < 9; j++) {
      for (let k = 0; k < 9; k++) {
        let count = 0;
        let rowIndex;
        for (let i = 0; i < 9; i++) {
          if (table[i][j].possibles[k]) {
            count++;
            rowIndex = i;
          }
        }
        if (count === 1 && table[rowIndex][j].certainty === 0) {
          table[rowIndex][j].certainty = k + 1;
          updateTable(k + 1, rowIndex, j, table, squares, squareList);
          console.log(`Certain by column: ${rowIndex} ${j} num:${k + 1}`);
          changed = true;
        }
      }
    }

    // Step 3.1: Check pairs in squares
    for (let square = 0; square < 9; square++) {
      possiblePairs = [];
      for (let k = 0; k < 9; k++) {
        let count = 0;
        let item;
        for (
          let i = squareList[square].startRow;
          i < squareList[square].endRow;
          i++
        ) {
          for (
            let j = squareList[square].startCol;
            j < squareList[square].endCol;
            j++
          ) {
            if (table[i][j].possibles[k] && table[i][j].certainty === 0) {
              count++;
              if (count === 1) {
                item = new TwoOccurrences(i, j, 0, 0, k + 1);
              } else if (count === 2) {
                item.row2 = i;
                item.col2 = j;
              }
            }
          }
        }
        if (count === 1) {
          table[item.row1][item.col1].certainty = k + 1;
          updateTable(k + 1, item.row1, item.col1, table, squares, squareList);
          console.log(
            `Certain by square: ${item.row1} ${item.col1} num:${k + 1}`
          );
          changed = true;
        } else if (count === 2) {
          possiblePairs.push(item);
        }
      }

      // Process pairs
      for (let a = 0; a < possiblePairs.length; a++) {
        for (let b = a + 1; b < possiblePairs.length; b++) {
          if (
            possiblePairs[a].row1 === possiblePairs[b].row1 &&
            possiblePairs[a].col1 === possiblePairs[b].col1 &&
            possiblePairs[a].row2 === possiblePairs[b].row2 &&
            possiblePairs[a].col2 === possiblePairs[b].col2
          ) {
            console.log(
              "3.1. pair1: " +
                possiblePairs[a].row1 +
                " " +
                possiblePairs[a].col1 +
                " pair2: " +
                possiblePairs[a].row2 +
                " " +
                possiblePairs[a].col2 +
                " numeros:" +
                possiblePairs[a].number +
                " " +
                possiblePairs[b].number
            );
            var aux =
              table[possiblePairs[a].row1][possiblePairs[a].col1]
                .numPossibilities;
            for (let i = 0; i < 9; i++) {
              if (
                i !== possiblePairs[a].number - 1 &&
                i !== possiblePairs[b].number - 1
              ) {
                table[possiblePairs[a].row1][possiblePairs[a].col1].possibles[
                  i
                ] = false;
              }
            }
            table[possiblePairs[a].row1][
              possiblePairs[a].col1
            ].countPossibilities();
            if (
              aux !==
              table[possiblePairs[a].row1][possiblePairs[a].col1]
                .numPossibilities
            ) {
              changed = true;
            }

            aux =
              table[possiblePairs[a].row2][possiblePairs[a].col2]
                .numPossibilities;
            for (let i = 0; i < 9; i++) {
              if (
                i !== possiblePairs[a].number - 1 &&
                i !== possiblePairs[b].number - 1
              ) {
                table[possiblePairs[a].row2][possiblePairs[a].col2].possibles[
                  i
                ] = false;
              }
            }
            table[possiblePairs[a].row2][
              possiblePairs[a].col2
            ].countPossibilities();
            if (
              aux !==
              table[possiblePairs[a].row2][possiblePairs[a].col2]
                .numPossibilities
            ) {
              changed = true;
            }
          }
        }
      }
    }

    // 3.2 verificar duas casas com apenas 2 opções, se
    // forem os mesmos números, tirar as possibilidades
    // desses 2 números em outras casas do mesmo square.
    for (let square = 0; square < 9; square++) {
      let possiblePairs = [];
      for (
        let i = squareList[square].startRow;
        i < squareList[square].endRow;
        i++
      ) {
        for (
          let j = squareList[square].startCol;
          j < squareList[square].endCol;
          j++
        ) {
          if (
            table[i][j].numPossibilities === 2 &&
            table[i][j].certainty === 0
          ) {
            possiblePairs.push(table[i][j]);
          }
        }
      }

      for (let a = 0; a < possiblePairs.length; a++) {
        for (let b = a + 1; b < possiblePairs.length; b++) {
          let pair1 = [];
          let pair2 = [];
          for (let c = 0; c < 9; c++) {
            if (possiblePairs[a].possibles[c]) {
              pair1.push(c);
            }
          }

          let d = 0;
          for (let c = 0; c < 9; c++) {
            if (possiblePairs[b].possibles[c] && pair1[d] === c) {
              pair2.push(c);
              d++;
            }
          }

          if (d === 2) {
            for (
              let i = squareList[square].startRow;
              i < squareList[square].endRow;
              i++
            ) {
              for (
                let j = squareList[square].startCol;
                j < squareList[square].endCol;
                j++
              ) {
                if (
                  (i !== possiblePairs[a].row || j !== possiblePairs[a].col) &&
                  (i !== possiblePairs[b].row || j !== possiblePairs[b].col)
                ) {
                  table[i][j].possibles[pair2[0]] = false;
                  table[i][j].possibles[pair2[1]] = false;
                  const aux = table[i][j].numPossibilities;
                  table[i][j].countPossibilities();
                  if (aux !== table[i][j].numPossibilities) {
                    changed = true;
                  }
                }
              }
            }
            console.log(`3.2. numbers ${pair2[0]} ${pair2[1]}`);
          }
        }
      }
    }

    // 4. verificar cada row/col se o número
    // aparece em apenas um dos squares, tirar
    // esse mesmo número das outras casas que não
    // sejam essa row/col.
    // by row
    for (let i = 0; i < 9; i++) {
      for (let k = 0; k < 9; k++) {
        let squas = [0, 0, 0];
        for (let j = 0; j < 9; j++) {
          if (table[i][j].possibles[k] && table[i][j].certainty === 0) {
            squas[Math.floor(j / 3)] = 1;
          }
        }
        if (squas[0] + squas[1] + squas[2] === 1) {
          for (let q = 0; q < 3; q++) {
            if (squas[q] === 1) {
              const square = squares[i][3 * q];
              console.log(`4. by row: ${i} square: ${square} num: ${k + 1}`);
              for (
                let row = squareList[square].startRow;
                row < squareList[square].endRow;
                row++
              ) {
                for (
                  let j = squareList[square].startCol;
                  j < squareList[square].endCol;
                  j++
                ) {
                  if (i !== row) {
                    if (table[row][j].possibles[k]) {
                      table[row][j].possibles[k] = false;
                      table[row][j].countPossibilities();
                      changed = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    // by column
    for (let j = 0; j < 9; j++) {
      for (let k = 0; k < 9; k++) {
        let squas = [0, 0, 0];
        for (let i = 0; i < 9; i++) {
          if (table[i][j].possibles[k] && table[i][j].certainty === 0) {
            squas[Math.floor(i / 3)] = 1;
          }
        }
        if (squas[0] + squas[1] + squas[2] === 1) {
          for (let q = 0; q < 3; q++) {
            if (squas[q] === 1) {
              const square = squares[3 * q][j];
              console.log(`4 by col: ${j} square: ${square} num: ${k + 1}`);
              for (
                let i = squareList[square].startRow;
                i < squareList[square].endRow;
                i++
              ) {
                for (
                  let col = squareList[square].startCol;
                  col < squareList[square].endCol;
                  col++
                ) {
                  if (j !== col) {
                    if (table[i][col].possibles[k]) {
                      table[i][col].possibles[k] = false;
                      table[i][col].countPossibilities();
                      changed = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    // 5. verificar se o número está em apenas
    // um sentido (row ou col). Se estiver,
    // tirar esse mesmo número dos outros squares.
    for (let square = 0; square < 9; square++) {
      for (let k = 0; k < 9; k++) {
        let cols = [0, 0, 0];
        let rows = [0, 0, 0];
        for (
          let i = squareList[square].startRow;
          i < squareList[square].endRow;
          i++
        ) {
          for (
            let j = squareList[square].startCol;
            j < squareList[square].endCol;
            j++
          ) {
            if (table[i][j].possibles[k] && table[i][j].certainty === 0) {
              cols[j - squareList[square].startCol] = 1;
              rows[i - squareList[square].startRow] = 1;
            }
          }
        }
        if (cols[0] + cols[1] + cols[2] === 1) {
          for (let col = 0; col < 3; col++) {
            if (cols[col] === 1) {
              for (let i = 0; i < 9; i++) {
                if (squares[i][col + squareList[square].startCol] !== square) {
                  if (
                    table[i][col + squareList[square].startCol].possibles[k]
                  ) {
                    table[i][col + squareList[square].startCol].possibles[
                      k
                    ] = false;
                    table[i][
                      col + squareList[square].startCol
                    ].countPossibilities();
                    changed = true;
                  }
                }
              }
              console.log(
                `5 by col ${col + squareList[square].startCol} num: ${k + 1}`
              );
            }
          }
        }
        if (rows[0] + rows[1] + rows[2] === 1) {
          for (let row = 0; row < 3; row++) {
            if (rows[row] === 1) {
              for (let j = 0; j < 9; j++) {
                if (squares[row + squareList[square].startRow][j] !== square) {
                  if (
                    table[row + squareList[square].startRow][j].possibles[k]
                  ) {
                    table[row + squareList[square].startRow][j].possibles[
                      k
                    ] = false;
                    table[row + squareList[square].startRow][
                      j
                    ].countPossibilities();
                    changed = true;
                  }
                }
              }
              console.log(
                `5 by row ${row + squareList[square].startRow} num: ${k + 1}`
              );
            }
          }
        }
      }
    }
  }

  // 7. se o loop repetiu sem nenhuma altera��o,
  // fazer um path com um chute em uma das casas
  // com 2 op��es. Quando n�o restar nenhuma op��o
  // com 100% verificar se n�o � um absurdo o chute
  // inicial.
  var finished = true;
  // Iterate through each row
  for (let i = 0; i < 9; i++) {
    // Iterate through each column
    for (let j = 0; j < 9; j++) {
      // If any cell's certainty is still 0, the Sudoku is not finished
      if (table[i][j].certainty !== 0) {
        var k = table[i][j].certainty;
        table[i][j].certainty = 0;
        var possible = true;
        possible = possible & !hasNumber(k, true, i, table);
        possible = possible & !hasNumber(k, false, j, table);
        possible =
          possible & !hasNumberInSquare(k, squares[i][j], table, squareList);
        if (!possible) {
          return false;
        }
        table[i][j].certainty = k;
      } else {
        finished = false;
      }
    }
  }

  if (finished) {
    return true;
  } else {
    console.log("not finished");
    var alternative = 0;

    var backupTable = [];
    for (let i = 0; i < 9; i++) {
      var tablesRow = [];
      for (let j = 0; j < 9; j++) {
        tablesRow.push(new Cell());
      }
      backupTable.push(tablesRow);
    }
    deepCopyTable(backupTable, table);
    for (let i = 0; i < 9; i++) {
      for (let j = 0; j < 9; j++) {
        if (table[i][j].numPossibilities === 2 && table[i][j].certainty === 0) {
          for (let k = 0; k < 9; k++) {
            if (table[i][j].possibles[k]) {
              table[i][j].certainty = k + 1;
              for (let l = k + 1; l < 9; l++) {
                if (table[i][j].possibles[l]) {
                  alternative = l + 1;
                }
              }
              updateTable(k + 1, i, j, table, squares, squareList);
              if (resolve(table, squareList, squares)) {
                console.log(`7. ${i} ${j} fist option ${k + 1}`);
                return true;
              } else {
                console.log(`7. ${i} ${j} alternative ${alternative}`);
                deepCopyTable(table, backupTable);
                table[i][j].certainty = alternative;
                updateTable(alternative, i, j, table, squares, squareList);                
                return resolve(table, squareList, squares);
              }
            }
          }
        }
      }
    }
  }

  finished = true;
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      if (table[i][j].certainty != 0) {
        k = table[i][j].certainty;
        table[i][j].certainty = 0;
        var possible = true;
        possible = possible & !hasNumber(k, true, i, table);
        possible = possible & !hasNumber(k, false, j, table);
        possible =
          possible & !hasNumberInSquare(k, squares[i][j], table, squareList);
        if (!possible) {
          return false;
        }
        table[i][j].certainty = k;
      } else {
        finished = false;
      }
    }
  }

  return finished;
}

function sudokuSolve(input) {
  console.log("start main");

  var table = [];
  var squareList = [];
  var squares = [];
  var possible = false;

  for (let square = 0; square < 9; square++) {
    squareList.push(new Square(0, 0, 0, 0));
  }

  for (let i = 0; i < 9; i++) {
    var squaresList = [];
    var tablesRow = [];
    for (let j = 0; j < 9; j++) {
      squaresList.push(0);
      tablesRow.push(new Cell());
    }
    squares.push(squaresList);
    table.push(tablesRow);
  }

  squareList[0].startRow =
    squareList[1].startRow =
    squareList[2].startRow =
    squareList[0].startCol =
    squareList[3].startCol =
    squareList[6].startCol =
      0;
  squareList[0].endRow =
    squareList[1].endRow =
    squareList[2].endRow =
    squareList[0].endCol =
    squareList[3].endCol =
    squareList[6].endCol =
    squareList[3].startRow =
    squareList[4].startRow =
    squareList[5].startRow =
    squareList[1].startCol =
    squareList[4].startCol =
    squareList[7].startCol =
      3;
  squareList[1].endCol =
    squareList[4].endCol =
    squareList[7].endCol =
    squareList[2].startCol =
    squareList[5].startCol =
    squareList[8].startCol =
    squareList[3].endRow =
    squareList[4].endRow =
    squareList[5].endRow =
    squareList[6].startRow =
    squareList[7].startRow =
    squareList[8].startRow =
      6;
  squareList[2].endCol =
    squareList[5].endCol =
    squareList[8].endCol =
    squareList[6].endRow =
    squareList[7].endRow =
    squareList[8].endRow =
      9;

  for (let k = 0; k < 9; k++) {
    for (let i = squareList[k].startRow; i < squareList[k].endRow; i++) {
      for (let j = squareList[k].startCol; j < squareList[k].endCol; j++) {
        squares[i][j] = k;
      }
    }
  }

  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      console.log(`read ${i} ${j} = ${input[i][j]}`);
      table[i][j] = new Cell();
      table[i][j].certainty = input[i][j];
      table[i][j].row = i;
      table[i][j].col = j;
      table[i][j].square = squares[i][j];
    }
  }

  // 1. coloca todas as possibilidades
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      if (table[i][j].certainty === 0) {
        for (let k = 1; k <= 9; k++) {
          possible = true;
          possible = possible & !hasNumber(k, true, i, table);
          possible = possible & !hasNumber(k, false, j, table);
          possible =
            possible & !hasNumberInSquare(k, squares[i][j], table, squareList);
          table[i][j].possibles[k - 1] = possible;
        }
        table[i][j].countPossibilities();
      } else {
        table[i][j].possibles[table[i][j].certainty - 1] = true;
        table[i][j].numPossibilities = 1;
      }
    }
  }

  if (resolve(table, squareList, squares)) {
    console.log("GAME SOLVED");
  } else {
    console.log("NOT SOLVED");
  }

  // end
  console.log("result:");
  for (let i = 0; i < 9; i++) {
    var line = "";
    for (let j = 0; j < 9; j++) {
      line += `${table[i][j].certainty} `;
      document.getElementById('cell' + i + j).value = table[i][j].certainty;
    }
    console.log(line);
  }
}
