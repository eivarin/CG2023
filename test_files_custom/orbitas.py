average = int(input("Inserir a distância média: "))
dif = int(input("Inserir a diferença das distâncias: "))
time = int(input("Inserir o tempo a demorar: "))
highest = int(input("Inserir altura máxima: "))
lowest = int(input("Inserir altura mínima: "))


highest_x_z = average + dif
lowest_x_z = average - dif
new_average = (average**2 / 2)**0.5

height_mid = (highest + lowest) / 2
height_high_mid = (highest + height_mid) / 2
height_low_mid = (lowest + height_mid) / 2

print(f"""<translate time="{time}" align="true" segments="10000">
    <point x="{highest_x_z}" y="{highest}" z="0" />
    <point x="{new_average}" y="{height_high_mid}" z="{new_average}" />
    <point x="0" y="{height_mid}" z="{lowest_x_z}" />
    <point x="{-new_average}" y="{height_low_mid}" z="{new_average}" />
    <point x="{-highest_x_z}" y="{height_mid}" z="0" />
    <point x="{-new_average}" y="{height_low_mid}" z="{-new_average}" />
    <point x="0" y="{height_mid}" z="{-lowest_x_z}" />
    <point x="{new_average}" y="{height_high_mid}" z="{-new_average}" />
</translate>""")
