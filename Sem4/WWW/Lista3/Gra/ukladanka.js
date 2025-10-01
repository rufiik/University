const canvas = document.getElementById("puzzleCanvas");
const ctx = canvas.getContext("2d");
const imageInput = document.getElementById("image");
const rowsInput = document.getElementById("rows");
const colsInput = document.getElementById("cols");
const startButton = document.getElementById("start");
const resetButton = document.getElementById("reset");

const backToMenuBtn = document.getElementById("backToMenu");
const winMessage = document.getElementById("winMessage");
const playAgainBtn = document.getElementById("playAgain");
let image = new Image();
let rows = 3, cols = 3;
let tiles = [];
let emptyTile = { row: 0, col: 0 };
let tileWidth, tileHeight;
let initialTiles = [];

function initializeGame() {
    canvas.width = 600;
    canvas.height = 600;
    tileWidth = canvas.width / cols;
    tileHeight = canvas.height / rows;
    tiles = [];
    let count = 0;
    for (let row = 0; row < rows; row++) {
        tiles[row] = [];
        for (let col = 0; col < cols; col++) {
            if (row === emptyTile.row && col === emptyTile.col) {
                tiles[row][col] = null;
                emptyTile = { row, col };
            } else {
                tiles[row][col] = { originalRow: row, originalCol: col };
            }
        }
    }
    shuffleByMovesWithEmptyReset();
    initialTiles = JSON.parse(JSON.stringify(tiles));
    drawPuzzle();
    document.getElementById('controls').style.display = 'none';
}

function shuffleByMovesWithEmptyReset(moveCount = 1000) {
    for (let i = 0; i < moveCount; i++) {
        const { row, col } = emptyTile;
        const possibleMoves = [];

        if (row > 0) possibleMoves.push({ row: row - 1, col }); // góra
        if (row < rows - 1) possibleMoves.push({ row: row + 1, col }); // dół
        if (col > 0) possibleMoves.push({ row, col: col - 1 }); // lewo
        if (col < cols - 1) possibleMoves.push({ row, col: col + 1 }); // prawo

        const move = possibleMoves[Math.floor(Math.random() * possibleMoves.length)];
        tiles[emptyTile.row][emptyTile.col] = tiles[move.row][move.col];
        tiles[move.row][move.col] = null;
        emptyTile = { row: move.row, col: move.col };
    }
    // Na koniec – przesuń puste pole z powrotem do (0, 0)
    while (emptyTile.col > 0) {
        swapTiles(emptyTile.row, emptyTile.col, emptyTile.row, emptyTile.col - 1);
    }
    while (emptyTile.row > 0) {
        swapTiles(emptyTile.row, emptyTile.col, emptyTile.row - 1, emptyTile.col);
    }
}

function swapTiles(r1, c1, r2, c2) {
    [tiles[r1][c1], tiles[r2][c2]] = [tiles[r2][c2], tiles[r1][c1]];
    if (tiles[r1][c1] === null) {
        emptyTile = { row: r1, col: c1 };
    } else if (tiles[r2][c2] === null) {
        emptyTile = { row: r2, col: c2 };
    }
}

function drawPuzzle() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    for (let row = 0; row < rows; row++) {
        for (let col = 0; col < cols; col++) {
            const tile = tiles[row][col];
            const x = col * tileWidth;
            const y = row * tileHeight;

            if (tile) {
                ctx.drawImage(
                    image,
                    tile.originalCol * (image.width / cols),
                    tile.originalRow * (image.height / rows),
                    image.width / cols,
                    image.height / rows,
                    x,
                    y,
                    tileWidth,
                    tileHeight
                );
            } else {
                ctx.fillStyle = '#00BFFF';
                ctx.fillRect(x, y, tileWidth, tileHeight);
            }
            if (hoveredTile && hoveredTile.row === row && hoveredTile.col === col) {
                ctx.save();
                ctx.lineWidth = 6;
                ctx.strokeStyle = 'yellow';
                ctx.strokeRect(x + 2, y + 2, tileWidth - 4, tileHeight - 4);
                ctx.restore();
            }
        }
    }
}

function handleTileClick(x, y) {
    const clickedCol = Math.floor(x / tileWidth);
    const clickedRow = Math.floor(y / tileHeight);

    const dr = Math.abs(clickedRow - emptyTile.row);
    const dc = Math.abs(clickedCol - emptyTile.col);

    if ((dr === 1 && dc === 0) || (dr === 0 && dc === 1)) {
        // Zamień miejscami
        tiles[emptyTile.row][emptyTile.col] = tiles[clickedRow][clickedCol];
        tiles[clickedRow][clickedCol] = null;

        emptyTile = { row: clickedRow, col: clickedCol };
        saveGameState();
        drawPuzzle();
        checkWinCondition();
    }
}

function saveGameState() {
    const gameState = {
        rows,
        cols,
        tiles,
        emptyTile,
        imageSrc: image.src
    };
    localStorage.setItem('puzzleGameState', JSON.stringify(gameState));
}

function loadGameState() {
    const saved = localStorage.getItem('puzzleGameState');
    if (saved) {
        const state = JSON.parse(saved);
        rows = state.rows;
        cols = state.cols;
        tiles = state.tiles;
        emptyTile = state.emptyTile;
        image = new Image();
        image.src = state.imageSrc;
        image.onload = () => {
            resizeCanvas();
            drawPuzzle();
        };
        return true;
    }
    return false;
}

window.onload = () => {
    if (!loadGameState()) {
        image.onload = () => {
         
        };
    }
};
function backToMenu() {
    document.getElementById('controls').style.display = 'block';
    winMessage.style.display = 'none';
}
function checkWinCondition() {
    let isSolved = true;
    for (let row = 0; row < rows; row++) {
        for (let col = 0; col < cols; col++) {
            const tile = tiles[row][col];
            if (row == emptyTile.row && col === emptyTile.col) continue;
            if (!tile || tile.originalRow !== row || tile.originalCol !== col) {
                isSolved = false;
                break;
            }
        }
    }
    if (isSolved) {
        alert("Gratulacje! Ułożyłeś układankę!");
        backToMenu();
    }
}

function resizeCanvas() {
    const size = Math.min(window.innerWidth, window.innerHeight) * 0.9;
    canvas.width = size;
    canvas.height = size;
    tileWidth = canvas.width / cols;
    tileHeight = canvas.height / rows;
    drawPuzzle();
}

canvas.addEventListener('mousemove', (e) => {
    const rect = canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    const mouseY = e.clientY - rect.top;

    const col = Math.floor(mouseX / tileWidth);
    const row = Math.floor(mouseY / tileHeight);
    if (
        (row === emptyTile.row && Math.abs(col - emptyTile.col) === 1) ||
        (col === emptyTile.col && Math.abs(row - emptyTile.row) === 1)
    ) {
        hoveredTile = { row, col };
    } else {
        hoveredTile = null;
    }

    drawPuzzle();
});

imageInput.addEventListener('change', (e) => {
    const file = e.target.files[0];
    if (file) {
        const reader = new FileReader();
        reader.onload = (event) => {
            image.src = event.target.result;
        };
        reader.readAsDataURL(file);
    }
});

startButton.addEventListener('click', () => {
    rows = parseInt(rowsInput.value);
    cols = parseInt(colsInput.value);
    if (image.src) {
        initializeGame();
    } else {
        alert('Proszę wybrać obrazek!');
    }
});

resetButton.addEventListener('click', () => {
    localStorage.removeItem('puzzleGameState');
    initializeGame();
});

canvas.addEventListener('touchstart', (e) => {
    if (e.touches.length === 1) {
        const rect = canvas.getBoundingClientRect();
        const touch = e.touches[0];
        const touchX = touch.clientX - rect.left;
        const touchY = touch.clientY - rect.top;

        handleTileClick(touchX, touchY);
    }
    e.preventDefault();
});

canvas.addEventListener('click', (e) => {
    const rect = canvas.getBoundingClientRect();
    const clickX = e.clientX - rect.left;
    const clickY = e.clientY - rect.top;
    handleTileClick(clickX, clickY);
});

resetButton.addEventListener('click', () => {
    localStorage.removeItem('puzzleGameState');
    initializeGame();
});

backToMenuBtn.addEventListener('click', () => {
    document.getElementById('controls').style.display = 'block';
    winMessage.style.display = 'none';
});

window.addEventListener('resize', resizeCanvas);