// Lazy init: only run mermaid when there are diagrams on the page
document$.subscribe(function() {
    if(document.querySelector('.mermaid')){
        mermaid.initialize({startOnLoad:false});
        mermaid.run({querySelector:'.mermaid'});
    }
});
