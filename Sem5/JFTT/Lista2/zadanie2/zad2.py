"""
aaaa
''''''''
bbbb
"""

'''
aaaa

'''

# 
# aaaaa/

# 

def oczysc(str):

    str = str.strip()  # usuń początkowe lub końcowe białe znaki

    str = str.lower()  # zmień na małe litery

    return str


# To jest komentarz
print("#Hello # world")  # komentarz po kodzie
print('String z # wewnątrz')
x = '''
To nie jest komentarz # pomimo hasha
'''
y = """
Ani to # nie jest komentarz
"""
print("Koniec")  # ostatni komentarz
print(x)
print(y)