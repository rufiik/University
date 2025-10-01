const http = require('http');
const usersRoutes = require('./routes/userRoute');
const gamesRoutes = require('./routes/gameRoute');
const reviewsRoutes = require('./routes/reviewRoute');
const { authenticateToken, authorizeRole } = require('./authMiddleware');
const { handleLogin } = require('./controllers/authController');

const server = http.createServer((req, res) => {
  if (req.url === '/api/login' && req.method === 'POST') {
    handleLogin(req, res); // Endpoint logowania
  } else if (req.url.startsWith('/api/users')) {
    authenticateToken(req, res, () => {
      authorizeRole('admin')(req, res, () => {
        usersRoutes(req, res); // Tylko admin ma dostęp do użytkowników
      });
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

server.listen(3000, () => {
  console.log('Serwer działa na porcie 3000');
});