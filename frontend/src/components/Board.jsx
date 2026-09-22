import Square from './Square';

const RANKS = ['8', '7', '6', '5', '4', '3', '2', '1'];
const FILES = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];

export default function Board({ board, selected, lastMove, turn, inCheck, shake, onSquareClick }) {
  if (!board) {
    return (
      <div className="board-frame">
        <div className="board-placeholder">
          <div className="spinner" />
          <span className="full-loader-text">Connecting to chess engine…</span>
        </div>
      </div>
    );
  }

  return (
    <div className={`board-frame ${shake ? 'shake' : ''}`}>
      {/* Rank labels (left side, inside frame) */}
      <div className="board-coords board-coords-ranks" aria-hidden="true">
        {RANKS.map(r => <span key={r}>{r}</span>)}
      </div>

      {/* File labels (bottom, inside frame) */}
      <div className="board-coords board-coords-files" aria-hidden="true">
        {FILES.map(f => <span key={f}>{f}</span>)}
      </div>

      <div className="board-grid" role="grid" aria-label="Chess board">
        {board.map((row, r) =>
          row.map((symbol, c) => {
            const isLight    = (r + c) % 2 === 0;
            const isSelected = selected?.row === r && selected?.col === c;
            const isLastFrom = lastMove?.from?.row === r && lastMove?.from?.col === c;
            const isLastTo   = lastMove?.to?.row   === r && lastMove?.to?.col   === c;

            const isCurrentKing =
              (turn === 'WHITE' && symbol === 'K') ||
              (turn === 'BLACK' && symbol === 'k');

            return (
              <Square
                key={`${r}-${c}`}
                row={r}
                col={c}
                symbol={symbol}
                isLight={isLight}
                isSelected={isSelected}
                isLastMove={isLastFrom || isLastTo}
                inCheck={inCheck && isCurrentKing}
                onClick={() => onSquareClick(r, c)}
              />
            );
          })
        )}
      </div>
    </div>
  );
}
