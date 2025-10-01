const { handleGetReviews, handleCreateReview, handleDeleteReview, handleUpdateReview } = require('../controllers/reviewController');

module.exports = (req, res) => {
  if (req.method === 'GET') {
    handleGetReviews(req, res); 
  } else if (req.method === 'POST') {
    handleCreateReview(req, res); 
  } else if (req.method === 'DELETE') {
    handleDeleteReview(req, res); 
  } else if (req.method === 'PUT') {
    handleUpdateReview(req, res); 
  } else {
    res.writeHead(405, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Method Not Allowed' }));
  }
};