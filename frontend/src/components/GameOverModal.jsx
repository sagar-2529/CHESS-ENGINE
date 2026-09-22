// GameOverModal — shown when status is CHECKMATE, STALEMATE, or DRAW

export default function GameOverModal({ status, winner, onNewGame }) {
  let icon, headline, sub;

  if (status === 'CHECKMATE' && winner) {
    icon     = '♛';
    headline = (
      <>
        <span className="winner-name">
          {winner.charAt(0) + winner.slice(1).toLowerCase()}
        </span>{' '}wins
      </>
    );
    sub = 'by checkmate';
  } else if (status === 'STALEMATE') {
    icon     = '⚖';
    headline = 'Draw';
    sub      = 'by stalemate — no legal moves';
  } else {
    icon     = '⚖';
    headline = 'Draw';
    sub      = 'game ended in a draw';
  }

  return (
    <div className="modal-backdrop" role="dialog" aria-modal="true" aria-label="Game over">
      <div className="gameover-modal">
        <div className="gameover-icon" aria-hidden="true">{icon}</div>

        <div className="gameover-result">
          <span className="gameover-label">Game over</span>
          <h2 className="gameover-headline">{headline}</h2>
          <p className="gameover-sub">{sub}</p>
        </div>

        <div className="gameover-actions">
          <button
            className="btn btn-primary"
            onClick={onNewGame}
            id="btn-play-again"
            aria-label="Start a new game"
          >
            Play Again
          </button>
        </div>
      </div>
    </div>
  );
}
