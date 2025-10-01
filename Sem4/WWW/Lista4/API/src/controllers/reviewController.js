const { getReviews, createReview, deleteReview, updateReview } = require('../models/reviewsModel');
const pool = require('../database');
const handleDeleteReview = async (req, res) => {
  const reviewId = req.url.split('/').pop(); 
  try {
    const result = await deleteReview(reviewId);
    if (result.affectedRows === 0) {
      res.writeHead(404, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Review not found' }));
      return;
    }
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ message: 'Review deleted' }));
  } catch (error) {
    console.error('Error deleting review:', error);
    res.writeHead(500, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Internal Server Error' }));
  }
};

const handleUpdateReview = async (req, res) => {
  const reviewId = req.url.split('/').pop(); 
  let body = '';
  req.on('data', chunk => (body += chunk));
  req.on('end', async () => {
    try {
      const { content, rating } = JSON.parse(body);

      const [review] = await pool.query('SELECT * FROM reviews WHERE id = ?', [reviewId]);
      if (!review) {
        res.writeHead(404, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Review not found' }));
        return;
      }


      await updateReview(reviewId, content, rating);
      res.writeHead(200, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ message: 'Review updated' }));
    } catch (error) {
      console.error('Error updating review:', error);
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
  });
};

const handleGetReviews = async (req, res) => {
  try {
    const urlParts = req.url.split('/');
    const reviewId = urlParts[urlParts.length - 1];

    // Sprawdź, czy zapytanie dotyczy konkretnej recenzji
    if (!isNaN(reviewId)) {
      const [review] = await pool.query('SELECT * FROM reviews WHERE id = ?', [reviewId]);
      if (!review) {
        res.writeHead(404, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Review not found' }));
        return;
      }
      res.writeHead(200, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify(review));
      return;
    }

    // Jeśli nie podano ID, zwróć listę recenzji z paginacją, filtrowaniem i sortowaniem
    const url = new URL(req.url, `http://${req.headers.host}`);
    const page = parseInt(url.searchParams.get('page')) || 1; // Domyślnie strona 1
    const limit = parseInt(url.searchParams.get('limit')) || 10; // Domyślnie 10 wyników na stronę
    const sort = url.searchParams.get('sort') || 'id'; // Domyślnie sortowanie po `id`
    const order = url.searchParams.get('order') || 'ASC'; // Domyślnie rosnąco
    const ratingFilter = url.searchParams.get('rating'); // Opcjonalne filtrowanie po `rating`

    const reviews = await getReviews(page, limit, sort, order, ratingFilter);

    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(reviews));
  } catch (error) {
    console.error('Error fetching reviews:', error);
    res.writeHead(500, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Internal Server Error' }));
  }
};

const handleCreateReview = async (req, res) => {
  let body = '';
  req.on('data', chunk => (body += chunk));
  req.on('end', async () => {
    try {
      const { userId, gameId, content, rating } = JSON.parse(body);
          // Sprawdź, czy userId istnieje
      const [user] = await pool.query('SELECT id FROM users WHERE id = ?', [userId]);
      if (!user) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Bad Request: User does not exist' }));
        return;
      }

      // Sprawdź, czy gameId istnieje
      const [game] = await pool.query('SELECT id FROM games WHERE id = ?', [gameId]);
      if (!game) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Bad Request: Game does not exist' }));
        return;
      }
      if (!userId || !gameId || !content || !rating) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Bad Request: Missing required fields' }));
        return;
      }
      
      await createReview(userId, gameId, content, rating);
      res.writeHead(201, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ message: 'Review created' }));
    } catch (error) {
      console.error('Error creating review:', error);
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
  });
};

module.exports = {
  handleGetReviews,
  handleCreateReview,
  handleDeleteReview,
  handleUpdateReview,
};