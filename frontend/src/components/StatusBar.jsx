// StatusBar — shows turn, game status, check/checkmate/draw indicator

function StatusBadge({ status, inCheck, winner }) {
  if (status === 'CHECKMATE') {
    return <span className="status-badge badge-danger">✕ Checkmate</span>;
  }
  if (status === 'STALEMATE') {
    return <span className="status-badge badge-draw">= Stalemate</span>;
  }
  if (status === 'DRAW') {
    return <span className="status-badge badge-draw">= Draw</span>;
  }
  if (inCheck) {
    return <span className="status-badge badge-check">⚠ Check!</span>;
  }
  return <span className="status-badge badge-active">● In Progress</span>;
}

export default function StatusBar({ turn, status, inCheck, winner }) {
  const isOver    = status === 'CHECKMATE' || status === 'STALEMATE' || status === 'DRAW';
  const whiteActive = !isOver && turn === 'WHITE';
  const blackActive = !isOver && turn === 'BLACK';

  // Turn display text
  let turnText = '';
  if (!isOver) {
    turnText = turn === 'WHITE' ? 'White to move' : 'Black to move';
  } else if (status === 'CHECKMATE' && winner) {
    turnText = `${winner.charAt(0) + winner.slice(1).toLowerCase()} wins`;
  } else {
    turnText = 'Game over';
  }

  return (
    <div className="status-card">
      {/* Player rows */}
      <div className={`player-card ${blackActive ? 'active' : ''}`}>
        <div className="player-card-left">
          <div className="player-swatch player-swatch-black" />
          <div>
            <div className="player-name">Black</div>
          </div>
        </div>
        {blackActive && <div className="active-dot" />}
      </div>

      {/* Current status */}
      <div style={{ display: 'flex', flexDirection: 'column', gap: '6px' }}>
        <span className="status-heading">Game status</span>
        <div className="status-primary">
          <span className="status-primary-text">{turnText}</span>
          <StatusBadge status={status} inCheck={inCheck} winner={winner} />
        </div>
      </div>

      {/* White player */}
      <div className={`player-card ${whiteActive ? 'active' : ''}`}>
        <div className="player-card-left">
          <div className="player-swatch player-swatch-white" />
          <div>
            <div className="player-name">White</div>
          </div>
        </div>
        {whiteActive && <div className="active-dot" />}
      </div>
    </div>
  );
}
 