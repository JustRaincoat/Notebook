window.MathJax={tex:{inlineMath:[['$','$'],['\\(','\\)']],displayMath:[['$$','$$'],['\\[','\\]']],processEscapes:true}};

// Material navigation.instant（SPA 站内跳转）下，页面不重新加载，
// 必须订阅 document$ 在每次跳转后重新渲染公式
if (window.document$ && typeof document$.subscribe === 'function') {
  document$.subscribe(function () {
    if (window.MathJax && MathJax.typesetPromise) {
      MathJax.typesetPromise().catch(function (err) { console.warn('MathJax typeset failed:', err); });
    }
  });
}
