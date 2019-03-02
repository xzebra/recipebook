const int MAX_INGREDIENTS = 25;
const int MAX_STEPS = 20;

const char* remoteHost = "https://www.hatcook.com/";
const char *CAT_RICE = "arroces-paellas",
            *CAT_SALAD = "ensaladas",
            *CAT_PASTA = "pasta-pizzas",
            *CAT_GREENS = "verduras",
            *CAT_SOUPS = "sopas-pures-cremas",
            *CAT_LEGUMES = "legumbres",
            *CAT_MEAT = "carnes",
            *CAT_FISH = "pescados-mariscos",
            *CAT_DESSERTS = "postres";

struct Recipe {
    String name;
    bool done;
    int people;
    String ingredients[MAX_INGREDIENTS];
    String steps[MAX_STEPS];
};

// recent most punctuated recipes are published
// inside <div class="grid destination-grid-wrapper">
void scrapRecipes() {

}