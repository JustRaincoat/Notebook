window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\(', '\\)']],
    displayMath: [['$$', '$$'], ['\\[', '\\]']],
    processEscapes: true
  },
  chtml: {
    // 关键：字体指向本地，避免默认走 jsdelivr CDN（国内超时导致公式空白/缺字符）
    fontURL: '/assets/mathjax/output/chtml/fonts/woff-v2'
  },
  startup: {
    ready: function () {
      MathJax.startup.defaultReady();
      // MathJax 就绪后订阅 Material document$（instant 站内跳转后重新渲染公式）
      MathJax.startup.promise.then(function () {
        if (window.document$ && typeof document$.subscribe === 'function') {
          document$.subscribe(function () {
            MathJax.typesetPromise().catch(function (err) {
              console.warn('MathJax typeset failed:', err);
            });
          });
        }
      });
    }
  }
};
