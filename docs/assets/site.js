(function () {
    var root = document.documentElement;
    var body = document.body;
    var themeToggles = Array.prototype.slice.call(document.querySelectorAll("[data-theme-toggle]"));
    var mobileToggle = document.querySelector("[data-mobile-nav-toggle]");
    var mobilePanel = document.querySelector("[data-mobile-nav]");
    var apiSearch = document.querySelector("[data-api-search]");
    var apiCards = Array.prototype.slice.call(document.querySelectorAll("[data-method-card]"));
    var searchEmpty = document.querySelector("[data-search-empty]");
    var tocLinks = Array.prototype.slice.call(document.querySelectorAll(".toc-link[href^='#']"));
    var pageLinks = Array.prototype.slice.call(document.querySelectorAll("[data-page-link]"));
    var currentPage = body.getAttribute("data-page");
    var storedTheme = null;

    try {
        storedTheme = localStorage.getItem("bst-docs-theme");
    } catch (error) {
        storedTheme = null;
    }

    function getPreferredTheme() {
        if (storedTheme === "light" || storedTheme === "dark") {
            return storedTheme;
        }
        if (window.matchMedia && window.matchMedia("(prefers-color-scheme: dark)").matches) {
            return "dark";
        }
        return "light";
    }

    function applyTheme(theme) {
        root.setAttribute("data-theme", theme);
        themeToggles.forEach(function (themeToggle) {
            themeToggle.setAttribute("aria-pressed", theme === "dark" ? "true" : "false");
            themeToggle.setAttribute("aria-label", theme === "dark" ? "Switch to light theme" : "Switch to dark theme");
            themeToggle.textContent = theme === "dark" ? "Light" : "Dark";
        });
    }

    applyTheme(getPreferredTheme());

    themeToggles.forEach(function (themeToggle) {
        themeToggle.addEventListener("click", function () {
            var nextTheme = root.getAttribute("data-theme") === "dark" ? "light" : "dark";
            storedTheme = nextTheme;
            try {
                localStorage.setItem("bst-docs-theme", nextTheme);
            } catch (error) {
                // Ignore storage failures.
            }
            applyTheme(nextTheme);
        });
    });

    if (window.matchMedia) {
        var darkMedia = window.matchMedia("(prefers-color-scheme: dark)");
        if (darkMedia.addEventListener) {
            darkMedia.addEventListener("change", function () {
                if (storedTheme !== "light" && storedTheme !== "dark") {
                    applyTheme(getPreferredTheme());
                }
            });
        }
    }

    if (mobileToggle && mobilePanel) {
        mobileToggle.addEventListener("click", function () {
            var isOpen = mobilePanel.classList.toggle("is-open");
            mobileToggle.setAttribute("aria-expanded", isOpen ? "true" : "false");
        });
    }

    pageLinks.forEach(function (link) {
        if (link.getAttribute("data-page-link") === currentPage) {
            link.classList.add("is-active");
        }
    });

    function attachCopyBehavior(wrapper) {
        if (!wrapper || wrapper.getAttribute("data-copy-ready") === "true") {
            return;
        }

        var code = wrapper.querySelector("pre code");
        var button = wrapper.querySelector(".copy-button");

        if (!code || !button) {
            return;
        }

        wrapper.setAttribute("data-copy-ready", "true");

        button.addEventListener("click", function () {
            var text = code.textContent || "";
            var originalText = button.textContent;

            function setTemporaryLabel(labelText) {
                button.textContent = labelText;
                window.setTimeout(function () {
                    button.textContent = originalText;
                }, 1500);
            }

            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(text).then(function () {
                    setTemporaryLabel("Copied");
                }).catch(function () {
                    setTemporaryLabel("Failed");
                });
            } else {
                setTemporaryLabel("Unsupported");
            }
        });
    }

    function wrapCodeBlocks() {
        var blocks = Array.prototype.slice.call(document.querySelectorAll("pre"));

        blocks.forEach(function (pre) {
            if (pre.closest(".code-block")) {
                return;
            }

            var code = pre.querySelector("code");
            if (!code) {
                return;
            }

            var wrapper = document.createElement("div");
            wrapper.className = "code-block";

            var header = document.createElement("div");
            header.className = "code-header";

            var label = document.createElement("span");
            label.className = "code-language";
            label.textContent = pre.getAttribute("data-language") || code.getAttribute("data-language") || "Code";

            var button = document.createElement("button");
            button.type = "button";
            button.className = "copy-button";
            button.setAttribute("aria-label", "Copy code block");
            button.textContent = "Copy";

            header.appendChild(label);
            header.appendChild(button);

            pre.parentNode.insertBefore(wrapper, pre);
            wrapper.appendChild(header);
            wrapper.appendChild(pre);

            attachCopyBehavior(wrapper);
        });

        Array.prototype.slice.call(document.querySelectorAll(".code-block")).forEach(attachCopyBehavior);
    }

    function setupSearch() {
        if (!apiSearch || apiCards.length === 0) {
            return;
        }

        apiSearch.addEventListener("input", function () {
            var query = apiSearch.value.trim().toLowerCase();
            var visibleCount = 0;

            apiCards.forEach(function (card) {
                var haystack = (card.getAttribute("data-search") || card.textContent || "").toLowerCase();
                var matches = query === "" || haystack.indexOf(query) !== -1;
                card.hidden = !matches;
                if (matches) {
                    visibleCount += 1;
                }
            });

            if (searchEmpty) {
                searchEmpty.classList.toggle("is-visible", visibleCount === 0);
            }
        });
    }

    function setupTocHighlight() {
        if (tocLinks.length === 0) {
            return;
        }

        var sections = tocLinks
            .map(function (link) {
                var id = link.getAttribute("href").slice(1);
                var section = document.getElementById(id);
                return section ? { link: link, section: section } : null;
            })
            .filter(Boolean);

        if (sections.length === 0) {
            return;
        }

        var observer = new IntersectionObserver(function (entries) {
            entries.forEach(function (entry) {
                if (!entry.isIntersecting) {
                    return;
                }

                sections.forEach(function (item) {
                    item.link.classList.toggle("is-active", item.section === entry.target);
                });
            });
        }, {
            rootMargin: "-20% 0px -65% 0px",
            threshold: [0, 1]
        });

        sections.forEach(function (item) {
            observer.observe(item.section);
        });
    }

    wrapCodeBlocks();
    setupSearch();
    setupTocHighlight();
}());
