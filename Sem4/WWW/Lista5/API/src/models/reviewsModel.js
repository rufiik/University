const pool = require('../database');

const getReviews = async (page, limit, sort, order, ratingFilter) => {
  const offset = (page - 1) * limit; // Oblicz offset dla paginacji
  const filterQuery = ratingFilter ? 'WHERE rating = ?' : ''; // Dodaj warunek filtrowania, jeśli podano `rating`

  const query = `
    SELECT * FROM reviews
    ${filterQuery} 
    ORDER BY ${sort} ${order} 
    LIMIT ? OFFSET ?
  `;

  const params = [];
  if (ratingFilter) params.push(ratingFilter); // Dodaj wartość filtra do parametrów
  params.push(limit, offset); // Dodaj limit i offset do parametrów

  return pool.query(query, params);
};

const createReview = async (userId, gameId, content, rating) => {
  return pool.query(
    'INSERT INTO reviews (user_id, game_id, content, rating) VALUES (?, ?, ?, ?)',
    [userId, gameId, content, rating]
  );
};

const deleteReview = async (id) => {
  return pool.query('DELETE FROM reviews WHERE id = ?', [id]);
};

const updateReview = async (id, content, rating) => {
  return pool.query('UPDATE reviews SET content = ?, rating = ? WHERE id = ?', [content, rating, id]);
};

module.exports = { getReviews, createReview, deleteReview, updateReview };