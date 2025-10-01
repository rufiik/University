import React, { useEffect, useState } from 'react';

function Reviews({ setError }) {
  const [reviews, setReviews] = useState([]);
  const [content, setContent] = useState('');
  const [rating, setRating] = useState('');
  const [gameId, setGameId] = useState('');
  const [games, setGames] = useState([]);
  const [editId, setEditId] = useState(null);
  const [editContent, setEditContent] = useState('');
  const [editRating, setEditRating] = useState('');
  const [EditGameId,setEditGameId] = useState('');
  // PAGINACJA, SORTOWANIE, FILTROWANIE
  const [page, setPage] = useState(1);
  const [limit, setLimit] = useState(5);
  const [sort, setSort] = useState('id');
  const [order, setOrder] = useState('ASC');
  const [ratingFilter, setRatingFilter] = useState('');
  const [total, setTotal] = useState(0);
  const [userRole, setUserRole] = useState('');
  const [userId, setUserId] = useState(null);

useEffect(() => {
  const token = localStorage.getItem('token');
  if (token) {
    try {
      const payload = JSON.parse(atob(token.split('.')[1]));
      setUserRole(payload.role);
        setUserId(payload.id);
    } catch {
      setUserRole('');
    }
  }
}, []);
  // Pobierz gry do wyboru w recenzji
  const fetchGames = async () => {
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/games', {
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd pobierania gier');
      const data = await response.json();
      setGames(data);
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Pobierz recenzje z paginacją, sortowaniem i filtrowaniem
  const fetchReviews = async () => {
    try {
      const token = localStorage.getItem('token');
      let url = `http://localhost:3001/api/reviews?page=${page}&limit=${limit}&sort=${sort}&order=${order}`;
      if (ratingFilter) url += `&rating=${ratingFilter}`;
      const response = await fetch(url, {
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd pobierania recenzji');
      const data = await response.json();
      setReviews(data.reviews || data); // jeśli backend zwraca {reviews, total}, inaczej samo data
      setTotal(data.total || 0); // jeśli backend zwraca total
    } catch (err) {
      setError && setError(err.message);
    }
  };

  useEffect(() => {
    fetchGames();
    // eslint-disable-next-line
  }, []);

  useEffect(() => {
    fetchReviews();
    // eslint-disable-next-line
  }, [page, sort, order, ratingFilter]);

  // Dodawanie recenzji
  const handleAddReview = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const payload = JSON.parse(atob(token.split('.')[1]));
      const userId = payload.id;
      const response = await fetch('http://localhost:3001/api/reviews', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ userId, gameId, content, rating }),
      });
      if (!response.ok) throw new Error('Błąd dodawania recenzji');
      setContent('');
      setRating('');
      setGameId('');
      fetchReviews();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Usuwanie recenzji
  const handleDeleteReview = async (id) => {
    if (!window.confirm('Na pewno usunąć tę recenzję?')) return;
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/reviews/${id}`, {
        method: 'DELETE',
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd usuwania recenzji');
      fetchReviews();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Rozpocznij edycję recenzji
  const startEdit = (review) => {
    setEditId(review.id || review._id);
    setEditContent(review.content);
    setEditRating(review.rating);
    setEditGameId(review.gameId || review.game_id);
  };

  // Zapisz edytowaną recenzję
  const handleEditReview = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/reviews/${editId}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ content: editContent, rating: editRating }),
      });
      if (!response.ok) throw new Error('Błąd edycji recenzji');
      setEditId(null);
      setEditContent('');
      setEditRating('');
      setEditGameId('');
      fetchReviews();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Obsługa zmiany strony
  const handlePrevPage = () => setPage(p => Math.max(1, p - 1));
  const handleNextPage = () => setPage(p => p + 1);

  return (
    <div className="main-container">
      <h2>Recenzje</h2>
      <form onSubmit={handleAddReview}>
        <select
          value={gameId}
          onChange={e => setGameId(e.target.value)}
          required
        >
          <option value="">Wybierz grę</option>
          {games.map(game => (
            <option key={game.id || game._id} value={game.id || game._id}>
              {game.title}
            </option>
          ))}
        </select>
        <input
          type="text"
          placeholder="Treść recenzji"
          value={content}
          onChange={e => setContent(e.target.value)}
          required
        />
        <input
          type="number"
          placeholder="Ocena (1-10)"
          value={rating}
          min="1"
          max="10"
          onChange={e => setRating(e.target.value)}
          required
        />
        <button type="submit">Dodaj recenzję</button>
      </form>

      {/* FILTROWANIE I SORTOWANIE */}
      <div>
        <label>
    Ile na stronę: 
    <select value={limit} onChange={e => { setLimit(Number(e.target.value)); setPage(1); }}>
        <option value={5}>5</option>
        <option value={10}>10</option>
        <option value={20}>20</option>
    </select>
    </label>

        <label>
          Filtrowanie po ocenie:
          <input
            type="number"
            min="1"
            max="5"
            value={ratingFilter}
            onChange={e => setRatingFilter(e.target.value)}
          />
        </label>
        <button
        type="button"
        style={{ marginLeft: 8 }}
        onClick={() => setRatingFilter('')}
        disabled={ratingFilter === ''}
        >
        Resetuj filtr
        </button>
        <label>
          Sortuj po:
          <select value={sort} onChange={e => setSort(e.target.value)} >
            <option value="id">ID</option>
            <option value="rating">Ocena</option>
          </select>
          <select value={order} onChange={e => setOrder(e.target.value)}>
            <option value="ASC">Rosnąco</option>
            <option value="DESC">Malejąco</option>
          </select>
        </label>
      </div>

      <ul>
        {reviews.map(review => (
          <li key={review.id || review._id}>
            {editId === (review.id || review._id) ? (
              <form onSubmit={handleEditReview} >
                <input
                  type="text"
                  value={editContent}
                  onChange={e => setEditContent(e.target.value)}
                  required
                />
                <input
                  type="number"
                  value={editRating}
                  min="1"
                  max="10"
                  onChange={e => setEditRating(e.target.value)}
                  required
                />
                <button type="submit">Zapisz</button>
                <button type="button" onClick={() => setEditId(null)}>Anuluj</button>
              </form>
            ) : (
              <>
                <b>Gra:</b> {
                  games.find(g => String(g.id || g._id) === String(review.gameId || review.game_id))?.title
                  || review.gameId || review.game_id
                } <br />
                <b>Treść:</b> {review.content} <br />
                <b>Ocena:</b> {review.rating}
                {(review.userId === userId || review.user_id === userId || userRole === 'admin') && (
                <button onClick={() => startEdit(review)}>Edytuj</button>
                )}
                {(review.userId === userId || review.user_id === userId || userRole === 'admin') && (
                <button onClick={() => handleDeleteReview(review.id || review._id)}>Usuń</button>
                )}
              </>
            )}
          </li>
        ))}
      </ul>

      {/* PAGINACJA */}
      <div>
        <button onClick={handlePrevPage} disabled={page === 1}>Poprzednia</button>
        <span>Strona: {page}</span>
        <button onClick={handleNextPage} disabled={reviews.length < limit}>Następna</button>
      </div>
    </div>
  );
}

export default Reviews;