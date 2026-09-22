import { useChessGame } from './hooks/useChessGame';
import Board from './components/Board';
import StatusBar from './components/StatusBar';
import GameControls from './components/GameControls';
import PromotionDialog from './components/PromotionDialog';
import GameOverModal from './components/GameOverModal';
import './index.css';

export default function App() {
  const {
    gameId,
    board,
    turn,
    status,
    inCheck,
    winner,
    selected,
    lastMove,
    loading,
    error,
    shake,
    pendingPromotion,
    onSquareClick,
    onPromotionChosen,
    newGame,
    resetGame,
  } = useChessGame();

  const isGameOver = status === 'CHECKMATE' || status === 'STALEMATE' || status === 'DRAW';

  return (
    <div className="app">
      {/* ── Header ───────────────────────────────────────────── */}
      <header className="app-header">
        <div className="app-header-brand">
          {/* Inline SVG pawn — always renders correctly regardless of font/OS */}
          <span className="app-header-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24" width="20" height="20" fill="currentColor" style={{ color: '#c8a96e' }}>
              <path d="M12 2a3.5 3.5 0 1 1 0 7 3.5 3.5 0 0 1 0-7zm0 8c1.5 0 2.8.7 3.6 1.8L17 15H7l1.4-3.2A4.5 4.5 0 0 1 12 10zm-5 6h10l1 3H6l1-3z"/>
            </svg>
          </span>
          <span className="app-header-title">
            Chess<span>.</span>
          </span>
        </div>
        <div className="app-header-meta">
          {gameId && (
            <button
              className="game-id-pill"
              onClick={() => navigator.clipboard?.writeText(gameId)}
              title="Copy game ID"
              aria-label="Copy game ID"
            >
              <span className="mono" style={{ fontSize: '0.72rem' }}>
                {gameId.slice(0, 10)}…
              </span>
              <span className="copy-icon" aria-hidden="true">⎘</span>
            </button>
          )}
        </div>
      </header>

      {/* ── Main Game Area ───────────────────────────────────── */}
      <main className="app-main">
        <div className="game-layout">
          {/* Board — dominant visual */}
          <section className="board-section" aria-label="Chess board">
            <Board
              board={board}
              selected={selected}
              lastMove={lastMove}
              turn={turn}
              inCheck={inCheck}
              shake={shake}
              onSquareClick={onSquareClick}
            />
          </section>

          {/* Side panel — status + controls */}
          <aside className="side-panel" aria-label="Game information">
            <StatusBar
              turn={turn}
              status={status}
              inCheck={inCheck}
              winner={winner}
            />
            <GameControls
              gameId={gameId}
              onReset={resetGame}
              onNewGame={newGame}
              loading={loading}
            />
          </aside>
        </div>
      </main>

      {/* ── Modals ───────────────────────────────────────────── */}
      {pendingPromotion && (
        <PromotionDialog turn={turn} onChoose={onPromotionChosen} />
      )}

      {isGameOver && (
        <GameOverModal
          status={status}
          winner={winner}
          onNewGame={newGame}
        />
      )}

      {/* ── Error Toast ──────────────────────────────────────── */}
      {error && (
        <div className="toast-container" role="alert" aria-live="assertive">
          <div className="toast">
            <span className="toast-icon" aria-hidden="true">⚠</span>
            <span className="toast-text">{error}</span>
          </div>
        </div>
      )}

      {/* ── Initial Loading Screen ───────────────────────────── */}
      {loading && !board && (
        <div className="full-loader" aria-busy="true" aria-label="Loading chess engine">
          <div className="spinner" />
          <span className="full-loader-text">Connecting to chess engine…</span>
        </div>
      )}
    </div>
  );
}
