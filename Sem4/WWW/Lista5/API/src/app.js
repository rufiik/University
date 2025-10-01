const http = require('http');
require('dotenv').config();
const usersRoutes = require('./routes/userRoute');
const gamesRoutes = require('./routes/gameRoute');
const reviewsRoutes = require('./routes/reviewRoute');
const { authenticateToken, authorizeRole } = require('./authMiddleware');
const { handleLogin } = require('./controllers/authController');

const server = http.createServer((req, res) => {
    res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type, Authorization');
  if (req.method === 'OPTIONS') {
    res.writeHead(204);
    res.end();
    return;
  }
  if (req.url === '/api/login' && req.method === 'POST') {
    handleLogin(req, res); // Endpoint logowania
  } else if (req.url.startsWith('/api/users')) {
    authenticateToken(req, res, () => {
        usersRoutes(req, res); 
    });
  } else if (req.url.startsWith('/api/games')) {
    authenticateToken(req, res, () => {
      gamesRoutes(req, res); // Wszyscy zalogowani użytkownicy mają dostęp do gier
    });
  } else if (req.url.startsWith('/api/reviews')) {
    authenticateToken(req, res, () => {
      reviewsRoutes(req, res); // Wszyscy zalogowani użytkownicy mają dostęp do recenzji
    });
  } else {
    res.writeHead(404, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Not Found' }));
  }
});

server.listen(3001, () => {
  console.log('Serwer działa na porcie 3000');
});